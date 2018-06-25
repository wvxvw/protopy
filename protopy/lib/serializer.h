#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <Python.h>
#include <apr_general.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _wbuffer {
    unsigned char* buf;
    size_t cap;
    size_t len;
    apr_pool_t* mp;
} wbuffer_t;

PyObject* proto_serialize(PyObject*, PyObject*);
    
#ifdef __cplusplus
}
#endif

#endif // SERIALIZER_H_
