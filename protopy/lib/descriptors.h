#ifndef DESCRIPTIORS_H_
#define DESCRIPTIORS_H_

#include <Python.h>
#include <apr_general.h>

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _kv {
    vt_type_t key;
    vt_type_t val;
    byte* pyval;
} kv_t;

typedef union _type_info {
    vt_type_t elt;
    kv_t pair;
} type_info_t;

typedef struct _field_info {
    size_t n;
    vt_type_t vt_type;
    // TODO(olegs): Make this const char* instead same for kv_t.pyval
    const byte* pytype;
    type_info_t extra_type_info;
} field_info_t;

typedef struct _factory {
    vt_type_t vt_type;
    apr_hash_t* mapping;
    PyObject* ctor;
} factory_t;

void extract_type_name(const byte*, apr_pool_t*, char**, char**);

void
enum_desc(
    const byte*,
    const list,
    apr_hash_t* const,
    PyObject*,
    apr_pool_t* const);

void
message_desc(
    const byte*,
    const list,
    apr_hash_t* const,
    PyObject*,
    apr_hash_t* const,
    apr_pool_t* const);

apr_hash_t*
create_descriptors(
    apr_hash_t* const,
    PyObject*,
    PyObject*,
    PyObject*,
    apr_pool_t* const);

#ifdef __cplusplus
}
#endif

#endif // DESCRIPTIORS_H_
