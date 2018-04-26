extern "C" {
#include <Python.h>
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL nnfc_codec_ARRAY_API
#include <numpy/arrayobject.h>
}

#include <exception>
#include <iostream>
#include <string>

#include "common.hh"
// TOOD(jremmons) use the stable API exposed by nnfc (once ready)
// rather than trying to compile against the internal header files
// that might change quickly. 
//#include "nnfc_API.hh"
#include "nnfc.hh"
#include "tensor.hh"
#include "nnfc_encoder.hh"

PyObject* NNFCEncoderContext_new(PyTypeObject *type, PyObject *, PyObject *) {

    NNFCEncoderContext *self;

    self = (NNFCEncoderContext*)type->tp_alloc(type, 0);
    if(self == NULL) {
        PyErr_SetString(PyExc_ValueError, "Could not alloc a new NNFCEncoderContext.");
        return 0;
    }

    return (PyObject*) self;
}

void NNFCEncoderContext_dealloc(NNFCEncoderContext* self) {

    Py_TYPE(self)->tp_free((PyObject*)self);
}

int NNFCEncoderContext_init(NNFCEncoderContext *self, PyObject *args, PyObject *) {

    char *codec_name = NULL;
    if (!PyArg_ParseTuple(args, "s", &codec_name)){
        return 0;
    }
        
    return 0;
}

PyObject* NNFCEncoderContext_encode(NNFCEncoderContext *self, PyObject *args){

    PyArrayObject *input_array;

    if (!PyArg_ParseTuple(args, "O", &input_array)){
        return 0;
    }
        
    try {

        auto input_tensors = array2tensor(input_array);

        // TODO(jremmons) call down to the libnnfc encode function
        // TODO(jremmons) munge the output into a 
        
        
        Py_INCREF(input_array);
        PyArrayObject *output_array = input_array;
        return reinterpret_cast<PyObject*>(output_array);
    }
    catch(std::exception e) {
        std::string error_message = e.what();
        PyErr_SetString(PyExc_Exception, error_message.c_str());
        return 0;
    }
    
}

PyObject* NNFCEncoderContext_backprop(NNFCEncoderContext *self, PyObject *args){

    Py_RETURN_NONE;
}

