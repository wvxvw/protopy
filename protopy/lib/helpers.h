#ifndef HELPERS_H_
#define HELPERS_H_

#include <apr_general.h>
#include "list.h"

#ifdef _WIN32
#define YY_NO_UNISTD_H
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

typedef enum ast_type_t {
    ast_message = 0,
    ast_enum = 1,
    ast_service = 3,
    ast_import = 4,
    ast_package = 5,
    ast_oneof = 6,
    ast_field = 7,
    ast_repeated = 8,
    ast_map = 9
} ast_type_t;

vt_type_t vt_builtin(const char*);

typedef struct _builtin_type {
    const char* name;
    const vt_type_t value;
} builtin_type_t;

#define BUILTIN_TYPES 16

typedef struct _proto_file_t {
    byte* package;
    list_t* imports;
    list_t* messages;
    list_t* enums;
    list_t* scope;
    list_t* current;
    list_t* previous;
    apr_pool_t* mp;
} proto_file_t;

proto_file_t* make_proto_file(apr_pool_t*);

byte* qualify_type(list_t*, proto_file_t*);

list_t* parse_import(byte*, apr_pool_t*);

char* unquote(char*);

#ifdef __cplusplus
}
#endif

#endif // HELPERS_H_
