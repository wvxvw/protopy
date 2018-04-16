%{
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "list.h"
#include "helpers.h"
#include "protopy.tab.h"

#define YY_DECL  int yylex \
    (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner)

#define YY_USER_ACTION                          \
    yylloc->first_line = yylloc->last_line;     \
    yylloc->first_column = yylloc->last_column; \
    for (int i = 0; yytext[i] != '\0'; i++) {   \
        if (yytext[i] == '\n') {                \
            yylloc->last_line++;                \
            yylloc->last_column = 0;            \
        }                                       \
        else {                                  \
            yylloc->last_column++;              \
        }                                       \
    }
%}

%option yylineno
%option bison-bridge
%option bison-locations
%option reentrant
%option noinput
%option nounput

%%

\"(\\([\\abfnrtv]|x[0-9a-fA-F][0-9a-fA-F]|[0-7][0-7][0-7])|[^"\\])*\" {
   yylval->string = strdup(yytext);
   return STRING_LITERAL;
}
'(\\([\\abfnrtv]|x[0-9a-fA-F][0-9a-fA-F]|[0-7][0-7][0-7])|[^'\\])*' {
   yylval->string = strdup(yytext);
   return STRING_LITERAL;
}

[1-9][0-9]* {
  yylval->string = strdup(yytext);
  return POSINTEGER;
}
[-][1-9][0-9]* {
  yylval->string = strdup(yytext);
  return NEGINTEGER;
}

true  { return BOOL_TRUE; }
false { return BOOL_FALSE; }

bool       { return BOOL; }
enum       { return ENUM; }
extensions { return EXTENSIONS; }
fixed32    { return FIXED32; }
fixed64    { return FIXED64; }
import     { return IMPORT; }
int32      { return INT32; }
int64      { return INT64; }
max        { return MAX; }
message    { return MESSAGE; }
map        { return MAP; }
oneof      { return ONEOF; }
option     { return OPTION; }
optional   { return OPTIONAL; }
package    { return PACKAGE; }
public     { return PUBLIC; }
required   { return REQUIRED; }
repeated   { return REPEATED; }
reserved   { return RESERVED; }
returns    { return RETURNS; }
rpc        { return RPC; }
service    { return SERVICE; }
sfixed32   { return SFIXED32; }
sfixed64   { return SFIXED64; }
sint32     { return SINT32; }
sint64     { return SINT64; }
string     { return STRING; }
stream     { return STREAM; }
syntax     { return SYNTAX; }
to         { return TO; }
uint32     { return UINT32; }
uint64     { return UINT64; }
weak       { return WEAK; }

[a-zA-Z][a-zA-Z0-9]* {
    yylval->string = strdup(yytext);
    return IDENTIFIER;
}

[(){}=\[\];,<>\.] { return *yytext; }
[ \n\t\r]+

%%

int yywrap(yyscan_t yyscanner)
{
    return 1;
}
