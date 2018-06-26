#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <Python.h>
#include <apr_general.h>
#include <apr_hash.h>

#include "descriptors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _wbuffer {
    unsigned char* buf;
    size_t cap;
    size_t len;
    size_t page_size;
    apr_pool_t* mp;
} wbuffer_t;

PyObject* proto_serialize(PyObject*, PyObject*);

void serialize_message(wbuffer_t*, PyObject*, factory_t*, apr_hash_t*, const char*);

void proto_serialize_impl(wbuffer_t*, const byte*, apr_hash_t* const, PyObject*);
    
#ifdef __cplusplus
}
#endif

#endif // SERIALIZER_H_
