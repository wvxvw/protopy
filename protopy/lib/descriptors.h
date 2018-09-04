#ifndef DESCRIPTIORS_H_
#define DESCRIPTIORS_H_

#include <Python.h>
#include <apr_general.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _kv {
    vt_type_t key;
    vt_type_t val;
    const char* pyval;
} kv_t;

typedef union _type_info {
    vt_type_t elt;
    kv_t pair;
} type_info_t;

typedef struct _field_info {
    size_t n;
    vt_type_t vt_type;
    const char* pytype;
    type_info_t extra_type_info;
} field_info_t;

typedef struct _factory {
    vt_type_t vt_type;
    apr_hash_t* mapping;
    const char* pytype;
    PyObject* ctor;
} factory_t;

apr_hash_t* create_descriptors(apr_hash_t* const, PyObject*, PyObject*, apr_pool_t* const);

#ifdef __cplusplus
}
#endif

#endif // DESCRIPTIORS_H_
