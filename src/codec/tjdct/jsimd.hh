#ifndef _JSMID_HH
#define _JSMID_HH

constexpr int16_t dct_table[] = {
    36,  50,  63,  113, 200, 157, 108, 55,  50,  85,  94,  215, 277,
    218, 150, 77,  63,  94,  191, 307, 261, 205, 141, 72,  113, 215,
    307, 277, 235, 185, 127, 65,  200, 277, 261, 235, 200, 157, 108,
    55,  157, 218, 205, 185, 157, 123, 85,  43,  108, 150, 141, 127,
    108, 85,  59,  30,  55,  77,  72,  65,  55,  43,  30,  15};

constexpr int16_t divisors[] = {
    -7282,  -23593, -31982, -28418, -23593, -12105, -26700, -27406, -23593,
    -15899, -20916, -26610, -5078,  -27056, -9612,  -10708, -31982, -20916,
    -21617, -10976, -1378,  -24716, -6253,  -7282,  -28418, -26610, -10976,
    -4859,  -29840, -20192, -32639, -1008,  -23593, -5078,  -1378,  -29840,
    -23593, -12105, -26700, -27406, -12105, -27056, -24716, -20192, -12105,
    -31574, -16191, -17326, -26700, -9612,  -6253,  -32639, -26700, -16191,
    -29687, -30583, -27406, -10708, -7282,  -1008,  -27406, -17326, -30583,
    -30583, 37,     51,     63,     113,    201,    157,    109,    56,
    51,     84,     95,     216,    278,    218,    151,    76,     63,
    95,     192,    308,    261,    206,    142,    73,     113,    216,
    308,    277,    236,    185,    127,    65,     201,    278,    261,
    236,    201,    157,    109,    56,     157,    218,    206,    185,
    157,    123,    85,     44,     109,    151,    142,    127,    109,
    85,     58,     30,     56,     76,     73,     65,     56,     44,
    30,     15,     1024,   1024,   1024,   512,    256,    256,    512,
    1024,   1024,   512,    512,    256,    128,    256,    256,    512,
    1024,   512,    256,    128,    128,    256,    256,    512,    512,
    256,    128,    128,    256,    256,    512,    512,    256,    128,
    128,    256,    256,    256,    512,    1024,   256,    256,    256,
    256,    256,    512,    512,    1024,   512,    256,    256,    512,
    512,    512,    1024,   2048,   1024,   512,    512,    512,    1024,
    1024,   2048,   4096,   6,      6,      6,      7,      8,      8,
    7,      6,      6,      7,      7,      8,      9,      8,      8,
    7,      6,      7,      8,      9,      9,      8,      8,      7,
    7,      8,      9,      9,      8,      8,      7,      7,      8,
    9,      9,      8,      8,      8,      7,      6,      8,      8,
    8,      8,      8,      7,      7,      6,      7,      8,      8,
    7,      7,      7,      6,      5,      6,      7,      7,      7,
    6,      6,      5,      4,      0,      0,      2,      0,      3,
    0,      4,      0,      5,      0,      6,      0,      14,     0,
    30,     0,      62,     0,      126,    0,      254,    0,      510,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0};

extern "C" const int jconst_fdct_ifast_sse2[];
extern "C" void jsimd_fdct_ifast_sse2(short *data);
extern "C" void jsimd_fdct_islow_sse2(short *data);

extern "C" void jsimd_quantize_sse2(short *coefs, const short *divisors,
                                    const short *data);

extern "C" const int jconst_idct_ifast_sse2[];
extern "C" void jsimd_idct_ifast_sse2(const void *dct_table,
                                      const short *coef_block,
                                      uint8_t **output_buf,
                                      unsigned int output_col);
extern "C" void jsimd_idct_islow_sse2(const void *dct_table,
                                      const short *coef_block,
                                      uint8_t **output_buf,
                                      unsigned int output_col);

#endif  // _JSMID_HH
