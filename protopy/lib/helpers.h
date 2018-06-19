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

#define YY_DECL  int yylex \
    (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner)

extern int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner);

#ifdef __cplusplus
}
#endif

#endif // HELPERS_H_
