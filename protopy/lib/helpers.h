#ifndef HELPERS_H_
#define HELPERS_H_

#include "list.h"
#include "protopy.tab.h"

#define YY_DECL  int yylex \
    (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner)

extern int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner);

#endif // HELPERS_H_
