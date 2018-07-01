#ifndef HELPERS_H_
#define HELPERS_H_

// TODO(olegs): Even though we don't use list.h, I cannot find a way
// to make protopy.tab.h to include it, so whenever we use it, we need
// to first include list.h...
#include "list.h"

#ifdef _WIN32
#define YY_NO_UNISTD_H
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#include "protopy.tab.h"

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

#define YY_DECL  int yylex \
    (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner)

extern int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner);

#ifdef __cplusplus
}
#endif

#endif // HELPERS_H_
