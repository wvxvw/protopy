%debug
%define api.pure full
%define api.push-pull push
%locations
%lex-param {void* scanner}
%parse-param {void* scanner}
%parse-param {list_t** result}
%parse-param {apr_pool_t* mp}

%{
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "helpers.h"
#include "list.h"
#include "protopy.tab.h"

void yyerror(YYLTYPE *locp, void* scanner, list_t** result, apr_pool_t* mp, char const *msg) {
    fprintf(stderr, "%d/%d: %s\n", locp->first_line, locp->first_column, msg);
}

list_t* tag(int t, list_t* body, apr_pool_t* mp) {
    return cons_int(t, 1, body, mp);
}

%}

%union {
    size_t keyword;
    int64_t index;
    char* string;
    list_t* object;
    void* nothing;
}
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
                keyword identifier built_in_type STRING_LITERAL option_name_body_part;

%type <object> string_literal oneof enum message service message_field 
               top_level s boolean literal message_block service_body
               service_body_part rpc top_levels enum_field enum_fields
               option_name type user_type user_type_tail rpc_type
               import field package_name package oneof_field oneof_fields
               map_field option_name_suffix option_name_body;

%type <nothing> syntax option_def field_options_body assignment
                range_end range ranges reserved_strings extend
                reserved_body reserved enum_value_option extensions rpc_options
                enum_value_options_group option_kv option_kvs option_value;

%type <index> import_kind field_label key_type positive_int;

%%

string_literal : STRING_LITERAL { $$ = cons_str($1, strlen($1), nil, mp); } ;

import_kind : WEAK { $$ = 1; }
            | PUBLIC { $$ = 2; }
            | %empty { $$ = 0; } ;


import : IMPORT import_kind string_literal ';' { $$ = $3; } ;


package_name : identifier {
    $$ = cons_str($1, strlen($1), nil, mp);
}
             | package_name '.' identifier {
    $$ = cons_str($3, strlen($3), $1, mp);
} ;

package : PACKAGE package_name ';' {
    $2 = nreverse($2);
    char* pname = mapconcat(to_str, $2, ".", mp);
    $$ = cons_str(pname, strlen(pname), nil, mp);
} ;


syntax : SYNTAX '=' string_literal ';' { $$ = NULL; } ;


user_type_tail : IDENTIFIER {
    $$ = cons_str($1, strlen($1), nil, mp);
}
          | user_type_tail '.' IDENTIFIER  {
    $$ = cons_str($3, strlen($3), $1, mp);
} ;

user_type : '.' user_type_tail {
    $$ = nappend($2, cons_str(".", 1, nil, mp));
}
          | user_type_tail ;


built_in_type : TBOOL    { $$ = "bool";       }
              | FIXED32  { $$ = "fixed32";    }
              | FIXED64  { $$ = "fixed64";    }
              | TINT32   { $$ = "int32";      }
              | TINT64   { $$ = "int64";      }
              | SFIXED32 { $$ = "sfixed32";   }
              | SFIXED64 { $$ = "sfixed64";   }
              | SINT32   { $$ = "sint32";     }
              | SINT64   { $$ = "sint64";     }
              | STRING   { $$ = "string";     }
              | TUINT32  { $$ = "uint32";     }
              | TUINT64  { $$ = "uint64";     } ;


type : built_in_type {
    $$ = cons_str($1, strlen($1), nil, mp);
}
     | user_type { $$ = reverse($$, mp); } ;


keyword : built_in_type
        | ENUM       { $$ = "enum";       }
        | EXTENSIONS { $$ = "extensions"; }
        | IMPORT     { $$ = "import";     }
        | MESSAGE    { $$ = "message";    }
        | MAP        { $$ = "map";        }
        | ONEOF      { $$ = "oneof";      }
        | OPTION     { $$ = "option";     }
        | TOPTIONAL  { $$ = "optional";   }
        | PACKAGE    { $$ = "package";    }
        | PUBLIC     { $$ = "public";     }
        | REQUIRED   { $$ = "required";   }
        | REPEATED   { $$ = "repeated";   }
        | RESERVED   { $$ = "reserved";   }
        | RETURNS    { $$ = "returns";    }
        | RPC        { $$ = "rpc";        }
        | SERVICE    { $$ = "service";    }
        | STREAM     { $$ = "stream";     }
        | SYNTAX     { $$ = "syntax";     }
        | TO         { $$ = "to";         }
        | WEAK       { $$ = "weak";       }
        | MAX        { $$ = "max";        } ;

identifier : keyword | IDENTIFIER ;


boolean : BOOL_TRUE { $$ = cons_int(1, sizeof(int), nil, mp); }
        | BOOL_FALSE { $$ = cons_int(0, sizeof(int), nil, mp); };


positive_int : POSINTEGER { $$ = (size_t)atoi($1); } ;

literal : NEGINTEGER { $$ = cons_int(atoi($1), sizeof(int), nil, mp); }
        | positive_int { $$ = cons_int($1, sizeof(int), nil, mp); }
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
    char* tname = mapconcat(to_str, $2, ".", mp);
    list_t* pos = cons_int($5, sizeof(int), nil, mp);
    list_t* idf = cons_str($3, strlen($3), pos, mp);
    int ftag;

    switch ($1) {
        case 2:
            ftag = 8;
            break;
        default:
            ftag = 7;
            break;
    }
    $$ = tag(ftag, cons_str(tname, strlen(tname), idf, mp), mp);
} ;

oneof_field : type identifier '=' positive_int field_options ';' {
    char* tname = mapconcat(to_str, $1, ".", mp);
    list_t* pos = cons_int($4, sizeof(int), nil, mp);
    list_t* idf = cons_str($2, strlen($2), pos, mp);
    $$ = tag(7, cons_str(tname, strlen(tname), idf, mp), mp);
} ;


oneof_fields : oneof_field {
    $$ = cons($1, tlist, nil, mp);
}            | oneof_fields oneof_field {
    $$ = cons($2, tlist, $1, mp);
} ;


oneof : ONEOF identifier '{' oneof_fields '}' {
    $$ = tag(6, cons_str($2, strlen($2), $4, mp), mp);
} ;


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
    char* vtype = mapconcat(to_str, $5, ".", mp);
    $$ = tag(9, cons(
                cons_int($3, 1, cons_str(vtype, strlen(vtype), nil, mp), mp),
                tlist,
                cons_str($7, strlen($7), cons_int($9, 1, nil, mp), mp), mp),
                mp);
} ;


range_end : MAX /* { $$ = SIZE_MAX; } */ { $$ = NULL; }
          | positive_int { $$ = NULL; } ;


range : positive_int TO range_end { $$ = NULL; }
      | positive_int { $$ = NULL; } ;


ranges : range | ranges ',' range { $$ = NULL; } ;


reserved_strings : string_literal { $$ = NULL; }
                 | reserved_strings ',' string_literal { $$ = NULL; } ;


reserved_body : ranges | reserved_strings ;


reserved : RESERVED reserved_body { $$ = NULL; } ;


option_name_body_part : IDENTIFIER | keyword ;

option_name_body : option_name_body_part {
    $$ = cons_str($1, strlen($1), nil, mp);
}
                 | option_name_body '.' option_name_body_part {
    $$ = cons($3, tstr, $1, mp);
} ;

option_name_suffix : %empty { $$ = nil; }
                   | '.' option_name_body { $$ = $2; }


option_name : '(' option_name_body ')' option_name_suffix {
    $$ = cons($4, tlist, $2, mp);
}
            | option_name_body ;


enum_value_option : option_name '=' literal { $$ = NULL; } ;


enum_value_options : enum_value_option
                   | enum_value_options ',' enum_value_option ;

enum_value_options_group : %empty { $$ = NULL; }
                         | '[' enum_value_options ']' { $$ = NULL; } ;


enum_field : identifier '=' positive_int enum_value_options_group {
    $$ = cons_str($1, strlen($1), cons_int($3, 1, nil, mp), mp);
}
           | OPTION option_name '=' literal {
    $$ = nil;
}
           | ';' { $$ = nil; } ;


enum_fields : enum_field {
    if (!null($1)) {
        $$ = from_lists(1, mp, $1);
    } else {
        $$ = nil;
    }
}
            | enum_fields enum_field {
    if (!null($2)) {
        $$ = cons($2, tlist, $1, mp);
    } else {
        $$ = $1;
    }
} ;


enum : ENUM identifier '{' enum_fields '}' {
    $$ = cons_str($2, strlen($2), $4, mp);
} ;


extend : EXTEND user_type '{' message_block '}' { $$ = NULL; }

extensions : EXTENSIONS range { $$ = NULL; } ;


message_field : enum              { $$ = tag(1, $1, mp); }
              | message           { $$ = tag(0, $1, mp); }
              | oneof
              | OPTION option_def { $$ = nil; }
              | map_field         { $$ = $1; }
              | field             { $$ = $1; }
              | reserved          { $$ = nil; }
              | extensions        { $$ = nil; }
              | extend            { $$ = nil; }
              | ';'               { $$ = nil; } ;


message_block : message_field {
    if (null($1)) {
        $$ = nil;
    } else {
        $$ = from_lists(1, mp, $1);
    }
}
              | message_block message_field {
    if (null($2)) {
        $$ = $1;
    } else {
        $$ = cons($2, tlist, $1, mp);
    }
} ;


message : MESSAGE identifier '{' message_block '}' {
    $$ = cons_str($2, strlen($2), $4, mp);
}       | MESSAGE identifier '{' '}' {
    $$ = cons_str($2, strlen($2), nil, mp);
};


rpc_type : STREAM type { $$ = $2; }
         | type ;


rpc_options : '{' OPTION option_def '}' { $$ = NULL; }
            | '{' '}' { $$ = NULL; }
            | %empty { $$ = NULL; } ;


rpc : identifier '(' rpc_type ')' RETURNS '(' rpc_type ')' rpc_options {
    $$ = cons_str($1, strlen($1), nil, mp);
} ;


service_body_part : OPTION option_def { $$ = nil; }
                  | RPC rpc           { $$ = $2; }
                  | ';'               { $$ = nil; };



service_body : %empty { $$ = from_lists(1, mp, nil); }
             | service_body service_body_part {
    $$ = cons($2, tlist, $1, mp);
} ;


service : SERVICE identifier '{' service_body '}' {
    $$ = cons_str($2, strlen($2), $4, mp);
} ;


top_level : message    { $$ = tag(0, $1, mp); }
          | enum       { $$ = tag(1, $1, mp); }
          | service    { $$ = tag(3, $1, mp); }
          | import     { $$ = tag(4, $1, mp); }
          | package    { $$ = tag(5, $1, mp); }
          | OPTION option_def { $$ = nil; }
          | extend     { $$ = nil; }
          | ';'        { $$ = nil; } ;


top_levels : %empty {
    $$ = from_lists(1, mp, nil);
}
           | top_levels top_level {
    if (null($2)) {
        $$ = $1;
    } else {
        $$ = cons($2, tlist, $1, mp);
    }
} ;


s : syntax top_levels {
    $$ = $2;
    *result = $$;
} ;

%%
