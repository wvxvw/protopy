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

typedef struct _htkv {
    unsigned short crc;
    const char* str;
} htkv_t;

#define KEYWORDS_SIZE 33

bool is_keyword(const byte*);

typedef enum vt_type_t {
    vt_uint32   = 0,
    vt_fixed32  = 1,
    vt_sint64   = 2,
    vt_int32    = 3,
    vt_sfixed64 = 4,
    vt_bool     = 5,
    vt_bytes    = 6,
    vt_double   = 7,
    vt_int64    = 8,
    vt_sint32   = 9,
    vt_string   = 10,
    vt_fixed64  = 11,
    vt_uint64   = 12,
    vt_sfixed32 = 13,

    // compound types
    vt_enum     = 14,
    vt_message  = 15,
    vt_repeated = 16,

    // extras
    vt_error    = 17,
    vt_default  = 18,
    vt_map      = 19
} vt_type_t;

vt_type_t vt_builtin(const char*);

#define BUILTIN_TYPES 14

typedef enum ast_type_t {
    ast_message  = 0,
    ast_enum     = 1,
    ast_service  = 3,
    ast_import   = 4,
    ast_package  = 5,
    ast_oneof    = 6,
    ast_field    = 7,
    ast_repeated = 8,
    ast_map      = 9
} ast_type_t;

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
