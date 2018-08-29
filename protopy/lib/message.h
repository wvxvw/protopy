#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <Python.h>
#include <apr_general.h>
#include <apr_strings.h>
#include <apr_hash.h>

#include "helpers.h"
#include "descriptors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _message message_t;

typedef struct _repteated_message repteated_message_t;

typedef struct _map_message map_message_t;

typedef struct _str {
    size_t n;
    const char* s;
} str_t;

typedef union _proto_value {
    uint32_t vuint32;
    uint32_t vfixed32;
    uint64_t vsint64;
    int32_t vint32;
    int64_t vsfixed64;
    bool vbool;
    str_t vbytes;
    uint64_t vdouble;
    int64_t vint64;
    int64_t vsint32;
    str_t vstring;
    uint64_t vfixed64;
    uint64_t vuint64;
    int64_t vsfixed32;
    uint64_t venum;
    message_t* vmessage;
    repteated_message_t* vrepeated;
    map_message_t* vmap;
    str_t vdefault;
} proto_value_t;

typedef struct _proto_fields {
    apr_array_header_t* /* of proto_value_t */ payload;
    apr_array_header_t* /* of field_info_t */ fields;
    apr_hash_t* mapping;
} proto_fields_t;

typedef union _proto_payload {
    proto_fields_t fields;
    str_t bytes;
} proto_payload_t;

typedef struct _message {
    bool t;
    proto_payload_t val;
} message_t;

typedef struct _repteated_message {
    size_t n;
    vt_type_t t;
    proto_value_t* val;
} repteated_message_t;

typedef struct _map_message {
    size_t n;
    vt_type_t t;
    proto_value_t* val;
} map_message_t;

typedef struct _pymessage {
    PyObject_HEAD;
    message_t* payload;
    factory_t* factory;
    apr_hash_t* mapping;
    // Wee need this to prevent Python from freeing the parser which
    // parsed this message.  If this parser is freed, there go also
    // the memory pools used to allocate this message.  Maybe, in the
    // future, we'll have some way of detecting the situation when the
    // parser is no longer used by Python and re-allocate all the
    // still existing messages in a new pool, but this seems extremely
    // complicated.
    PyObject* parser;
} pymessage_t;


PyObject* proto_message_from_bytes(PyObject*, PyObject*);

extern PyTypeObject* proto_message_type;


#ifdef __cplusplus
}
#endif

#endif // MESSAGE_H_
