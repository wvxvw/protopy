#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdbool.h>
#include <apr_general.h>
#include <apr_tables.h>

#ifdef _WIN32
#define YY_NO_UNISTD_H
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

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

typedef struct _proto_field {
    const char* name;
    const char* t;
    size_t n;
} proto_field_t;

typedef struct _proto_enum_member {
    const char* name;
    size_t n;
} proto_enum_member_t;

typedef struct _proto_map_field {
    const char* name;
    vt_type_t kt;
    const char* vt;
    size_t n;
} proto_map_field_t;

typedef struct _proto_message {
    const char* t;
    apr_array_header_t* fields;
    apr_array_header_t* repeated;
    apr_array_header_t* maps;
} proto_message_t;

typedef struct _proto_enum {
    const char* t;
    apr_array_header_t* members;
} proto_enum_t;

typedef struct _proto_file_t {
    char* package;
    apr_array_header_t* imports;
    apr_array_header_t* messages;
    apr_array_header_t* enums;
    apr_array_header_t* scope;
    proto_message_t* current_message;
    proto_enum_t* current_enum;
    apr_array_header_t* previous;
    bool need;
    apr_pool_t* mp;
} proto_file_t;

proto_field_t* make_proto_field(const char*, apr_array_header_t*, int, proto_file_t*);

proto_map_field_t* make_proto_map_field(const char*, int, apr_array_header_t*, int, proto_file_t*);

proto_enum_member_t* make_proto_enum_member(const char*, int, apr_pool_t*);

proto_enum_t* make_proto_enum(const char*, proto_file_t*);

proto_message_t* make_proto_message(apr_array_header_t*, proto_file_t*);

typedef struct _htkv {
    unsigned short crc;
    const char* str;
} htkv_t;

#define KEYWORDS_SIZE 33

bool is_keyword(const char*);

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

proto_file_t* make_proto_file(apr_pool_t*);

proto_file_t* proto_file_copy(proto_file_t*, apr_pool_t*);

char* qualify_type(apr_array_header_t*, proto_file_t*);

apr_array_header_t* parse_import(char*, apr_pool_t*);

char* unquote(char*);

char* implode_range(apr_array_header_t*, const char*, apr_pool_t*, size_t, size_t, size_t);

#ifdef __cplusplus
}
#endif

#endif // HELPERS_H_
