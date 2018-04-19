#include "nnfc.hh"

#include <turbojpeg.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <Eigen/CXX11/Tensor>

#include "blob.hh"
#include "blob1d.hh"
#include "blob4d.hh"

static uint64_t magic_num = 0xDEADBEEF;

// TODO(jremmons)
// put header information at the beginning of the blob
// add a get_size function
// put the file in protobuf format
// add a 'working memory' blob that can be pass into the function
// 

void NNFC::encode(const Blob<float, 4> &input_blob, Blob<uint8_t, 1> &output_blob) {

    auto input = input_blob.get_tensor();
    
    static_assert(sizeof(double) == sizeof(uint64_t), "the current code assumes doubles are 64-bit long");
    size_t metadata_length = 6*sizeof(uint64_t);

    uint64_t batch_size = input.dimension(0);
    uint64_t channels = input.dimension(1);
    uint64_t height = input.dimension(2);
    uint64_t width = input.dimension(3);

    std::cerr << batch_size << " " << channels << " " << height << " " << width << std::endl;
    
    output_blob.resize(sizeof(uint8_t) * batch_size * channels * height * width + metadata_length);
    auto output = output_blob.get_tensor();
    
    float max_valf = 0.0;
    float min_valf = 0.0;

    // get the min and the max values
    for(size_t n = 0; n < batch_size; n++){
        for(size_t i = 0; i < channels; i++){
            for(size_t j = 0; j < height; j++){
                for(size_t k = 0; k < width; k++){

                    float num = input(n, i ,j ,k); 
                    if(num > max_valf){
                        max_valf = num;
                    }
                    if(num < min_valf){
                        min_valf = num;
                    }

                }
            }
        }
    }
    
    assert(max_valf > 0);
    assert(min_valf == 0);
    std::cerr << "min_val: " << min_valf  << " max_val: " << max_valf << std::endl;

    for(size_t n = 0; n < batch_size; n++){
        for(size_t i = 0; i < channels; i++){
            for(size_t j = 0; j < height; j++){
                for(size_t k = 0; k < width; k++){

                    float num = input(n, i ,j ,k); 

                    num = num / (max_valf/255.0); // squash between 0 and 255.0
                    uint8_t quantized_val = static_cast<uint8_t>(num);
                    
                    size_t offset = channels*height*width*n + height*width*i + width*j + k;
                    output(offset) = quantized_val;

                }
            }
        }
    }
    
    // const int JPEG_QUALITY = 98;
    // const int COLOR_COMPONENTS = 1;

    // tjhandle _jpegCompressor = tjInitCompress();

    // int _width = 1920;
    // int _height = 1080;
    // long unsigned int _jpegSize = 0;
    // unsigned char* _compressedImage = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0
    // //unsigned char buffer[_width*_height*COLOR_COMPONENTS]; //!< Contains the uncompressed image

    // tjCompress2(_jpegCompressor, buffer, _width, 0, _height, TJPF_RGB,
    //             &_compressedImage, &_jpegSize, TJSAMP_444, JPEG_QUALITY,
    //             TJFLAG_FASTDCT);
    
    // tjDestroy(_jpegCompressor);

    double max_val = max_valf;
    
    uint8_t *magic_num_bytes = reinterpret_cast<uint8_t*>(&magic_num);
    uint8_t *batch_size_bytes = reinterpret_cast<uint8_t*>(&batch_size);
    uint8_t *channels_bytes = reinterpret_cast<uint8_t*>(&channels);
    uint8_t *height_bytes = reinterpret_cast<uint8_t*>(&height);
    uint8_t *width_bytes = reinterpret_cast<uint8_t*>(&width);
    uint8_t *max_val_bytes = reinterpret_cast<uint8_t*>(&max_val);
    
    for(size_t i = 0; i < 8; i++){
        output(output.size()-8 + i) = magic_num_bytes[i];
        output(output.size()-16 + i) = batch_size_bytes[i];
        output(output.size()-24 + i) = channels_bytes[i];
        output(output.size()-32 + i) = height_bytes[i];
        output(output.size()-40 + i) = width_bytes[i];    
        output(output.size()-48 + i) = max_val_bytes[i];
    }
    
}

void NNFC::decode(const Blob<uint8_t, 1> &input_blob, Blob<float, 4> &output_blob) {

    auto input = input_blob.get_tensor();

    uint64_t magic_num_read;
    uint64_t batch_size;
    uint64_t channels;
    uint64_t height;
    uint64_t width;
    double max_val;

    uint8_t *magic_num_bytes = reinterpret_cast<uint8_t*>(&magic_num_read);
    uint8_t *batch_size_bytes = reinterpret_cast<uint8_t*>(&batch_size);
    uint8_t *channels_bytes = reinterpret_cast<uint8_t*>(&channels);
    uint8_t *height_bytes = reinterpret_cast<uint8_t*>(&height);
    uint8_t *width_bytes = reinterpret_cast<uint8_t*>(&width);
    uint8_t *max_val_bytes = reinterpret_cast<uint8_t*>(&max_val);

    for(size_t i = 0; i < 8; i++){
        magic_num_bytes[i] = input(input.size()-8 + i);
        batch_size_bytes[i] = input(input.size()-16 + i);
        channels_bytes[i] = input(input.size()-24 + i);
        height_bytes[i] = input(input.size()-32 + i);
        width_bytes[i] = input(input.size()-40 + i);    
        max_val_bytes[i] = input(input.size()-48 + i);    
    }

    if(magic_num_read != magic_num) {
        std::cerr << "magic number does not match!" << std::endl;
        throw std::runtime_error("magic number does not match!");
    }

    output_blob.resize(batch_size, channels, height, width);
    auto output = output_blob.get_tensor();
    
    for(size_t n = 0; n < batch_size; n++){
        for(size_t i = 0; i < channels; i++){
            for(size_t j = 0; j < height; j++){
                for(size_t k = 0; k < width; k++){

                    size_t offset = channels*height*width*n + height*width*i + width*j + k;
                    uint8_t quantized_val = input(offset);
                    
                    float uncompressed_val = (max_val * static_cast<double>(quantized_val)) / 255.0;

                    output(n, i, j, k) = uncompressed_val;

                }
            }
        }
    }

}
