#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

PyObject* proto_serialize(PyObject*, PyObject*);
    
#ifdef __cplusplus
}
#endif

#endif // SERIALIZER_H_
