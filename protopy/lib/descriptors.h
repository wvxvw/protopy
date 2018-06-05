#ifndef DESCRIPTIORS_H_
#define DESCRIPTIORS_H_

#include <apr_general.h>
#include <Python.h>

#include "list.h"

typedef enum vt_type_t {
    // varint
    vt_int32  = 0,
    vt_int64  = 1,
    vt_uint32 = 2,
    vt_uint64 = 3,
    vt_sint32 = 4,
    vt_sint64 = 5,
    vt_bool   = 6,
    vt_enum   = 7,
    // 64 bit
    vt_fixed64  = 8,
    vt_sfixed64 = 9,
    vt_double   = 10,
    // length-delimited
    vt_string   = 11,
    vt_bytes    = 12,
    vt_message  = 13,
    vt_repeated = 14,
    // 32 bit
    vt_fixed32  = 15,
    vt_sfixed32 = 16,
    // not exposed to user
    vt_error   = 17,
    vt_default = 18,
    vt_map = 19
} vt_type_t;

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

#endif // DESCRIPTIORS_H_
