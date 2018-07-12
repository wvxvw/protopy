%debug
%define api.pure full
%define api.push-pull push
%locations
%lex-param {void* scanner}
%parse-param {void* scanner}
%parse-param {proto_file_t* pf}

%code requires {
#include <apr_general.h>
#include "helpers.h"

union YYSTYPE {
    size_t keyword;
    int index;
    char* string;
    apr_array_header_t* array;
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
#include <apr_strings.h>

#include "protopy.tab.h"

void yyerror(YYLTYPE *locp, void* scanner, proto_file_t* pf, char const *msg) {
    fprintf(stderr, "%d/%d: %s\n", locp->first_line, locp->first_column, msg);
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

%type <array> type user_type user_type_tail package_name;

%type <nothing> syntax option_def field_options_body assignment package
                range_end range ranges reserved_strings extend import oneof
                reserved_body reserved enum_value_option extensions rpc_options
                enum_value_options_group option_kv option_kvs option_value
                map_field option_name_suffix option_name enum_fields enum
                message_field rpc_type enum_field top_level s message_block
                service_body service_body_part rpc top_levels field oneof_field
                oneof_fields option_name_body message service message_tail literal
                string_literal;

%type <index> import_kind field_label key_type positive_int boolean;

%%

string_literal : STRING_LITERAL { $$ = NULL; } ;

import_kind : WEAK { $$ = 1; }
            | PUBLIC { $$ = 2; }
            | %empty { $$ = 0; } ;


import : IMPORT import_kind STRING_LITERAL ';' {
    // TODO(olegs): Imorts need to be a prefix tree.
    APR_ARRAY_PUSH(pf->imports, apr_array_header_t*) = parse_import($3, pf->mp);
} ;


package_name : identifier {
    $$ = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH($$, char*) = $1;
}
             | package_name '.' identifier {
    APR_ARRAY_PUSH($1, char*) = $3;
    $$ = $1;
} ;

package : PACKAGE package_name ';' {
    pf->package = apr_array_pstrcat(pf->mp, $2, '.');
    $$ = NULL;
} ;


syntax : SYNTAX '=' string_literal ';' { $$ = NULL; } ;


user_type_tail : IDENTIFIER {
    $$ = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH($$, char*) = $1;
}
          | user_type_tail '.' IDENTIFIER  {
    APR_ARRAY_PUSH($$, char*) = $3;
    $$ = $1;
} ;

user_type : '.' user_type_tail {
    $$ = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH($$, char*) = ".";
    apr_array_cat($$, $2);
}
          | user_type_tail ;


built_in_type : TBOOL    { $$ = apr_pstrdup(pf->mp, "bool");       }
              | FIXED32  { $$ = apr_pstrdup(pf->mp, "fixed32");    }
              | FIXED64  { $$ = apr_pstrdup(pf->mp, "fixed64");    }
              | TINT32   { $$ = apr_pstrdup(pf->mp, "int32");      }
              | TINT64   { $$ = apr_pstrdup(pf->mp, "int64");      }
              | SFIXED32 { $$ = apr_pstrdup(pf->mp, "sfixed32");   }
              | SFIXED64 { $$ = apr_pstrdup(pf->mp, "sfixed64");   }
              | SINT32   { $$ = apr_pstrdup(pf->mp, "sint32");     }
              | SINT64   { $$ = apr_pstrdup(pf->mp, "sint64");     }
              | STRING   { $$ = apr_pstrdup(pf->mp, "string");     }
              | TUINT32  { $$ = apr_pstrdup(pf->mp, "uint32");     }
              | TUINT64  { $$ = apr_pstrdup(pf->mp, "uint64");     } ;


type : built_in_type {
    $$ = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH($$, char*) = $1;
}
     | user_type ;


keyword : built_in_type
        | ENUM       { $$ = apr_pstrdup(pf->mp, "enum");       }
        | EXTENSIONS { $$ = apr_pstrdup(pf->mp, "extensions"); }
        | IMPORT     { $$ = apr_pstrdup(pf->mp, "import");     }
        | MESSAGE    { $$ = apr_pstrdup(pf->mp, "message");    }
        | MAP        { $$ = apr_pstrdup(pf->mp, "map");        }
        | ONEOF      { $$ = apr_pstrdup(pf->mp, "oneof");      }
        | OPTION     { $$ = apr_pstrdup(pf->mp, "option");     }
        | TOPTIONAL  { $$ = apr_pstrdup(pf->mp, "optional");   }
        | PACKAGE    { $$ = apr_pstrdup(pf->mp, "package");    }
        | PUBLIC     { $$ = apr_pstrdup(pf->mp, "public");     }
        | REQUIRED   { $$ = apr_pstrdup(pf->mp, "required");   }
        | REPEATED   { $$ = apr_pstrdup(pf->mp, "repeated");   }
        | RESERVED   { $$ = apr_pstrdup(pf->mp, "reserved");   }
        | RETURNS    { $$ = apr_pstrdup(pf->mp, "returns");    }
        | RPC        { $$ = apr_pstrdup(pf->mp, "rpc");        }
        | SERVICE    { $$ = apr_pstrdup(pf->mp, "service");    }
        | STREAM     { $$ = apr_pstrdup(pf->mp, "stream");     }
        | SYNTAX     { $$ = apr_pstrdup(pf->mp, "syntax");     }
        | TO         { $$ = apr_pstrdup(pf->mp, "to");         }
        | WEAK       { $$ = apr_pstrdup(pf->mp, "weak");       }
        | MAX        { $$ = apr_pstrdup(pf->mp, "max");        } ;

identifier : keyword | IDENTIFIER ;


boolean : BOOL_TRUE { $$ = 1; }
        | BOOL_FALSE { $$ = 0; };


positive_int : POSINTEGER {
    $$ = (int)apr_atoi64($1);
} ;

literal : NEGINTEGER     { $$ = NULL; }
        | positive_int   { $$ = NULL; }
        | string_literal { $$ = NULL; }
        | boolean        { $$ = NULL; }
        | user_type      { $$ = NULL; } ;

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
    proto_field_t* field = make_proto_field($3, $2, $5, pf);
    apr_array_header_t* arr = ($1 == 2) ?
        pf->current_message->repeated :
        pf->current_message->fields;
    APR_ARRAY_PUSH(arr, proto_field_t*) = field;
    $$ = NULL;
} ;

oneof_field : type identifier '=' positive_int field_options ';' {
    APR_ARRAY_PUSH(pf->current_message->fields, proto_field_t*) =
        make_proto_field($2, $1, $4, pf);
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
    APR_ARRAY_PUSH(pf->current_message->maps, proto_map_field_t*) =
        make_proto_map_field($7, $3, $5, $9, pf);
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
    APR_ARRAY_PUSH(pf->current_enum->members, proto_enum_member_t*) =
        make_proto_enum_member($1, $3, pf->mp);
    $$ = NULL;
}
           | OPTION option_name '=' literal { $$ = NULL; }
           | ';'                            { $$ = NULL; } ;


enum_fields : enum_field             { $$ = NULL; }
            | enum_fields enum_field { $$ = NULL; } ;


enum : ENUM identifier {
    pf->current_enum = make_proto_enum(pf->scope, $2, pf->mp);
} '{' enum_fields '}' {
    APR_ARRAY_PUSH(pf->enums, proto_enum_t*) = pf->current_enum;
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
    APR_ARRAY_PUSH(pf->scope, char*) = $1;
    APR_ARRAY_PUSH(pf->previous, proto_message_t*) = pf->current_message;
    pf->current_message = make_proto_message(pf->scope, pf);
} message_tail {
    APR_ARRAY_PUSH(pf->messages, proto_message_t*) = pf->current_message;
    pf->current_message = apr_array_pop(pf->previous);
    apr_array_pop(pf->scope);
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
