%debug
%define api.pure full
%define api.push-pull push
%locations
%lex-param {void* scanner}
%parse-param {void* scanner}
%parse-param {proto_file_t* pf}

%code requires {
#include <apr_general.h>
#include "list.h"
#include "helpers.h"

union YYSTYPE {
    size_t keyword;
    int index;
    byte* string;
    list_t* object;
    void* nothing;
};

typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1

typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};

#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1

#define YY_DECL  int yylex \
    (YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner, apr_pool_t* mp)

extern int yylex(
    YYSTYPE* yylval_param,
    YYLTYPE* yylloc_param,
    void* yyscanner,
    apr_pool_t* mp);
}

%{
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "helpers.h"
#include "list.h"
#include "protopy.tab.h"

void yyerror(YYLTYPE *locp, void* scanner, proto_file_t* pf, char const *msg) {
    fprintf(stderr, "%d/%d: %s\n", locp->first_line, locp->first_column, msg);
}

list_t* tag(int t, list_t* body, apr_pool_t* mp) {
    return cons_int(t, 1, body, mp);
}

%}

%start s;
%token TBOOL;
%token BOOL_FALSE;
%token BOOL_TRUE;
%token ENUM;
%token EXTENSIONS;
%token EXTEND;
%token FIXED32;
%token FIXED64;
%token IMPORT;
%token TINT32;
%token TINT64;
%token MAP;
%token MAX;
%token MESSAGE;
%token ONEOF;
%token OPTION;
%token TOPTIONAL;
%token PACKAGE;
%token PUBLIC;
%token REPEATED;
%token REQUIRED;
%token RESERVED;
%token RETURNS;
%token RPC;
%token SERVICE;
%token SFIXED32;
%token SFIXED64;
%token SINT32;
%token SINT64;
%token STREAM;
%token STRING;
%token STRING_LITERAL;
%token SYNTAX;
%token TO;
%token TUINT32;
%token TUINT64;
%token WEAK;

%token POSINTEGER;
%token NEGINTEGER;
%token IDENTIFIER;

%type <keyword>  TBOOL ENUM EXTENSIONS BOOL_FALSE FIXED32 FIXED64 IMPORT TINT32
                 TINT64 MAP MAX MESSAGE ONEOF OPTION TOPTIONAL PACKAGE PUBLIC
                 REPEATED REQUIRED RESERVED RETURNS RPC SERVICE SFIXED32
                 SFIXED64 SINT32 SINT64 STREAM SYNTAX TO BOOL_TRUE TUINT32
                 TUINT64 WEAK STRING;

%type <string>  POSINTEGER NEGINTEGER IDENTIFIER
                keyword identifier built_in_type STRING_LITERAL option_name_body_part
                message_header;

%type <object> string_literal boolean literal type user_type user_type_tail
               package_name;

%type <nothing> syntax option_def field_options_body assignment package
                range_end range ranges reserved_strings extend import oneof
                reserved_body reserved enum_value_option extensions rpc_options
                enum_value_options_group option_kv option_kvs option_value
                map_field option_name_suffix option_name enum_fields enum
                message_field rpc_type enum_field top_level s message_block
                service_body service_body_part rpc top_levels field oneof_field
                oneof_fields option_name_body message service message_tail;

%type <index> import_kind field_label key_type positive_int;

%%

string_literal : STRING_LITERAL { $$ = ncons_str($1, nil, pf->mp); } ;

import_kind : WEAK { $$ = 1; }
            | PUBLIC { $$ = 2; }
            | %empty { $$ = 0; } ;


import : IMPORT import_kind STRING_LITERAL ';' {
    // TODO(olegs): Imorts need to be a prefix tree.
    pf->imports = cons(parse_import($3, pf->mp), tlist, pf->imports, pf->mp);
} ;


package_name : identifier {
    $$ = ncons_str($1, nil, pf->mp);
}
             | package_name '.' identifier {
    $$ = ncons_str($3, $1, pf->mp);
} ;

package : PACKAGE package_name ';' {
    $2 = nreverse($2);
    pf->package = nmapconcat(to_bytes, $2, ".", pf->mp);
    $$ = NULL;
} ;


syntax : SYNTAX '=' string_literal ';' { $$ = NULL; } ;


user_type_tail : IDENTIFIER {
    $$ = ncons_str($1, nil, pf->mp);
}
          | user_type_tail '.' IDENTIFIER  {
    $$ = ncons_str($3, $1, pf->mp);
} ;

user_type : '.' user_type_tail {
    $$ = nappend($2, cons_str(".", 1, nil, pf->mp));
}
          | user_type_tail ;


built_in_type : TBOOL    { $$ = cstr_bytes("bool", pf->mp);       }
              | FIXED32  { $$ = cstr_bytes("fixed32", pf->mp);    }
              | FIXED64  { $$ = cstr_bytes("fixed64", pf->mp);    }
              | TINT32   { $$ = cstr_bytes("int32", pf->mp);      }
              | TINT64   { $$ = cstr_bytes("int64", pf->mp);      }
              | SFIXED32 { $$ = cstr_bytes("sfixed32", pf->mp);   }
              | SFIXED64 { $$ = cstr_bytes("sfixed64", pf->mp);   }
              | SINT32   { $$ = cstr_bytes("sint32", pf->mp);     }
              | SINT64   { $$ = cstr_bytes("sint64", pf->mp);     }
              | STRING   { $$ = cstr_bytes("string", pf->mp);     }
              | TUINT32  { $$ = cstr_bytes("uint32", pf->mp);     }
              | TUINT64  { $$ = cstr_bytes("uint64", pf->mp);     } ;


type : built_in_type {
    $$ = ncons_str($1, nil, pf->mp);
}
     | user_type { $$ = nreverse($$); } ;


keyword : built_in_type
        | ENUM       { $$ = cstr_bytes("enum", pf->mp);       }
        | EXTENSIONS { $$ = cstr_bytes("extensions", pf->mp); }
        | IMPORT     { $$ = cstr_bytes("import", pf->mp);     }
        | MESSAGE    { $$ = cstr_bytes("message", pf->mp);    }
        | MAP        { $$ = cstr_bytes("map", pf->mp);        }
        | ONEOF      { $$ = cstr_bytes("oneof", pf->mp);      }
        | OPTION     { $$ = cstr_bytes("option", pf->mp);     }
        | TOPTIONAL  { $$ = cstr_bytes("optional", pf->mp);   }
        | PACKAGE    { $$ = cstr_bytes("package", pf->mp);    }
        | PUBLIC     { $$ = cstr_bytes("public", pf->mp);     }
        | REQUIRED   { $$ = cstr_bytes("required", pf->mp);   }
        | REPEATED   { $$ = cstr_bytes("repeated", pf->mp);   }
        | RESERVED   { $$ = cstr_bytes("reserved", pf->mp);   }
        | RETURNS    { $$ = cstr_bytes("returns", pf->mp);    }
        | RPC        { $$ = cstr_bytes("rpc", pf->mp);        }
        | SERVICE    { $$ = cstr_bytes("service", pf->mp);    }
        | STREAM     { $$ = cstr_bytes("stream", pf->mp);     }
        | SYNTAX     { $$ = cstr_bytes("syntax", pf->mp);     }
        | TO         { $$ = cstr_bytes("to", pf->mp);         }
        | WEAK       { $$ = cstr_bytes("weak", pf->mp);       }
        | MAX        { $$ = cstr_bytes("max", pf->mp);        } ;

identifier : keyword | IDENTIFIER ;


boolean : BOOL_TRUE { $$ = cons_int(1, sizeof(int), nil, pf->mp); }
        | BOOL_FALSE { $$ = cons_int(0, sizeof(int), nil, pf->mp); };


positive_int : POSINTEGER {
    $$ = atoi(nbytes_cstr($1));
} ;

literal : NEGINTEGER { $$ = cons_int(atoi(nbytes_cstr($1)), sizeof(int), nil, pf->mp); }
        | positive_int { $$ = cons_int($1, sizeof(int), nil, pf->mp); }
        | string_literal
        | boolean
        | user_type ;

option_kv : identifier ':' literal { $$ = NULL; }
          | option_name '{' option_kvs '}' { $$ = NULL; } ;

option_kvs : option_kv
           | option_kvs option_kv ;

option_value : literal { $$ = NULL; }
             | '{' option_kvs '}' { $$ = NULL; }
             | '{' '}' {
    /* protoc has a bug where it allows option declaration without value */
    $$ = NULL;
 };


assignment : option_name '=' option_value { $$ = NULL; } ;


option_def : assignment ';' { $$ = NULL; } ;


field_options_body : assignment | field_options_body ',' assignment ;


field_options : '[' field_options_body ']' | %empty;


field_label : REQUIRED  { $$ = 1; }
            | REPEATED  { $$ = 2; }
            | TOPTIONAL { $$ = 3; }
            | %empty    { $$ = 0; } ;


field : field_label type identifier '=' positive_int field_options ';' {
    byte* tname = qualify_type($2, pf);
    list_t* pos = cons_int($5, sizeof(int), nil, pf->mp);
    list_t* idf = ncons_str($3, pos, pf->mp);
    int ftag = ($1 == 2) ? ast_repeated : ast_field;
    list_t* field = tag(ftag, ncons_str(tname, idf, pf->mp), pf->mp);
    pf->current = cons(field, tlist, pf->current, pf->mp);
    $$ = NULL;
} ;

oneof_field : type identifier '=' positive_int field_options ';' {
    byte* tname = qualify_type($1, pf);
    list_t* pos = cons_int($4, sizeof(int), nil, pf->mp);
    list_t* idf = ncons_str($2, pos, pf->mp);
    list_t* field = tag(ast_field, ncons_str(tname, idf, pf->mp), pf->mp);
    pf->current = cons(field, tlist, pf->current, pf->mp);
    $$ = NULL;
} ;


oneof_fields : oneof_field
             | oneof_fields oneof_field ;


oneof : ONEOF identifier '{' oneof_fields '}' { $$ = NULL; } ;


key_type : TINT32   { $$ = vt_int32;    }
         | TINT64   { $$ = vt_int64;    }
         | TUINT32  { $$ = vt_uint32;   }
         | TUINT64  { $$ = vt_uint64;   }
         | SINT32   { $$ = vt_sint32;   }
         | SINT64   { $$ = vt_sint64;   }
         | FIXED32  { $$ = vt_fixed32;  }
         | FIXED64  { $$ = vt_fixed64;  }
         | SFIXED32 { $$ = vt_sfixed32; }
         | SFIXED64 { $$ = vt_sfixed64; }
         | TBOOL    { $$ = vt_bool;     }
         | STRING   { $$ = vt_string;   } ;


map_field : MAP '<' key_type ',' type '>' identifier '='
            positive_int field_options ';' {
    byte* vtype = nmapconcat(to_bytes, $5, ".", pf->mp);
    list_t* ftype = cons_int($3, sizeof(int), ncons_str(vtype, nil, pf->mp), pf->mp);
    list_t* fname = ncons_str($7, cons_int($9, sizeof(int), nil, pf->mp), pf->mp);
    list_t* field = tag(ast_map, cons(ftype, tlist, fname, pf->mp), pf->mp);
    pf->current = cons(field, tlist, pf->current, pf->mp);
    $$ = NULL;
} ;


range_end : MAX /* { $$ = SIZE_MAX; } */ { $$ = NULL; }
          | positive_int { $$ = NULL; } ;


range : positive_int TO range_end { $$ = NULL; }
      | positive_int { $$ = NULL; } ;


ranges : range | ranges ',' range { $$ = NULL; } ;


reserved_strings : string_literal                      { $$ = NULL; }
                 | reserved_strings ',' string_literal { $$ = NULL; } ;


reserved_body : ranges | reserved_strings ;


reserved : RESERVED reserved_body { $$ = NULL; } ;


option_name_body_part : IDENTIFIER | keyword ;

option_name_body : option_name_body_part                      { $$ = NULL; }
                 | option_name_body '.' option_name_body_part { $$ = NULL; } ;

option_name_suffix : %empty               { $$ = NULL; }
                   | '.' option_name_body { $$ = NULL; }


option_name : '(' option_name_body ')' option_name_suffix { $$ = NULL; }
            | option_name_body ;


enum_value_option : option_name '=' literal { $$ = NULL; } ;


enum_value_options : enum_value_option
                   | enum_value_options ',' enum_value_option ;

enum_value_options_group : %empty { $$ = NULL; }
                         | '[' enum_value_options ']' { $$ = NULL; } ;


enum_field : identifier '=' positive_int enum_value_options_group {
    list_t* field = ncons_str($1, cons_int($3, sizeof(int), nil, pf->mp), pf->mp);
    pf->current = cons(field, tlist, pf->current, pf->mp);
    $$ = NULL;
}
           | OPTION option_name '=' literal { $$ = NULL; }
           | ';'                            { $$ = NULL; } ;


enum_fields : enum_field             { $$ = NULL; }
            | enum_fields enum_field { $$ = NULL; } ;


enum : ENUM identifier {
    pf->scope = ncons_str($2, pf->scope, pf->mp);
    pf->previous = cons(pf->current, tlist, pf->previous, pf->mp);
    pf->current = nil;
} '{' enum_fields '}' {
    byte* enum_type = qualify_type(reverse(pf->scope, pf->mp), pf);
    list_t* enumerator = tag(ast_enum, ncons_str(enum_type, pf->current, pf->mp), pf->mp);
    pf->enums = cons(enumerator, tlist, pf->enums, pf->mp);
    pf->current = LIST_VAL(pf->previous);
    pf->previous = cdr(pf->previous);
    pf->scope = cdr(pf->scope);
    $$ = NULL;
} ;


extend : EXTEND user_type '{' message_block '}' { $$ = NULL; }


extensions : EXTENSIONS range { $$ = NULL; } ;


message_field : enum
              | message
              | oneof
              | OPTION option_def { $$ = NULL; }
              | map_field
              | field
              | reserved
              | extensions
              | extend
              | ';' { $$ = NULL; } ;


message_block : message_field { $$ = NULL; }
              | message_block message_field { $$ = NULL; } ;


message_header : MESSAGE identifier '{' { $$ = $2; } ;


message_tail : message_block '}' { $$ = NULL; }
             | '}'               { $$ = NULL; };

message : message_header {
    pf->scope = ncons_str($1, pf->scope, pf->mp);
    pf->previous = cons(pf->current, tlist, pf->previous, pf->mp);
    pf->current = nil;
} message_tail {
    byte* message_type = qualify_type(reverse(pf->scope, pf->mp), pf);
    list_t* message = tag(ast_message, ncons_str(message_type, pf->current, pf->mp), pf->mp);
    pf->messages = cons(message, tlist, pf->messages, pf->mp);
    pf->current = LIST_VAL(pf->previous);
    pf->previous = cdr(pf->previous);
    pf->scope = cdr(pf->scope);
};


rpc_type : STREAM type { $$ = NULL; }
         | type        { $$ = NULL; } ;


rpc_options : '{' OPTION option_def '}' { $$ = NULL; }
            | '{' '}'                   { $$ = NULL; }
            | %empty                    { $$ = NULL; } ;


rpc : identifier '(' rpc_type ')' RETURNS '(' rpc_type ')' rpc_options {
    $$ = NULL;
} ;


service_body_part : OPTION option_def { $$ = NULL; }
                  | RPC rpc           { $$ = NULL; }
                  | ';'               { $$ = NULL; };



service_body : %empty                         { $$ = NULL; }
             | service_body service_body_part { $$ = NULL; } ;


service : SERVICE identifier '{' service_body '}' { $$ = NULL; } ;


top_level : message
          | enum
          | service
          | import
          | package
          | OPTION option_def { $$ = NULL; }
          | extend
          | ';'               { $$ = NULL; };


top_levels : %empty               { $$ = NULL; }
           | top_levels top_level { $$ = NULL; } ;


s : syntax top_levels { $$ = NULL; } ;

%%
