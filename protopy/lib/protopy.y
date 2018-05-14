%debug
%define api.pure full
%define api.push-pull push
%locations
%lex-param {void* scanner}
%parse-param {void* scanner}
%parse-param {list* result}

%{
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "helpers.h"
#include "list.h"
#include "protopy.tab.h"

void yyerror(YYLTYPE *locp, void* scanner, list* result, char const *msg) {
    fprintf(stderr, "%d/%d: %s\n", locp->first_line, locp->first_column, msg);
}

list tag(int t, list body) {
    return cons_int(t, 1, body);
}

#define MAYBE_ABORT                             \
do {                                            \
    if (false) {                                \
        YYABORT;                                \
    }                                           \
} while (0)
%}

%union {
    size_t keyword;
    int64_t index;
    char* string;
    list object;
    void* nothing;
}
%start s;
%token BOOL;
%token BOOL_FALSE;
%token BOOL_TRUE;
%token ENUM;
%token EXTENSIONS;
%token FIXED32;
%token FIXED64;
%token IMPORT;
%token INT32;
%token INT64;
%token MAP;
%token MAX;
%token MESSAGE;
%token ONEOF;
%token OPTION;
%token OPTIONAL;
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
%token UINT32;
%token UINT64;
%token WEAK;

%token POSINTEGER;
%token NEGINTEGER;
%token IDENTIFIER;

%type <keyword>  BOOL ENUM EXTENSIONS BOOL_FALSE FIXED32 FIXED64 IMPORT INT32
                 INT64 MAP MAX MESSAGE ONEOF OPTION OPTIONAL PACKAGE PUBLIC
                 REPEATED REQUIRED RESERVED RETURNS RPC SERVICE SFIXED32
                 SFIXED64 SINT32 SINT64 STREAM SYNTAX TO BOOL_TRUE UINT32
                 UINT64 WEAK STRING;

%type <string>  POSINTEGER NEGINTEGER IDENTIFIER
                keyword identifier built_in_type STRING_LITERAL;

%type <object> string_literal oneof enum message service message_field 
               top_level s boolean literal message_block service_body
               service_body_part rpc top_levels enum_field enum_fields
               option_name_parts option_name type user_type rpc_type
               import field package_name package oneof_field oneof_fields;

%type <nothing> syntax option_def field_options_body assignment
                map_field range_end range ranges reserved_strings
                reserved_body reserved enum_value_option extensions rpc_options
                enum_value_options_group;

%type <index> import_kind field_label key_type positive_int;

%%

string_literal : STRING_LITERAL { MAYBE_ABORT; $$ = cons_str($1, strlen($1), nil); } ;

import_kind : WEAK { MAYBE_ABORT; $$ = 1; }
            | PUBLIC { MAYBE_ABORT; $$ = 2; }
            | %empty { MAYBE_ABORT; $$ = 0; } ;


import : IMPORT import_kind string_literal ';' { MAYBE_ABORT; $$ = $3; } ;


package_name : identifier {
    MAYBE_ABORT;
    $$ = cons_str($1, strlen($1), nil);
    printf("package name: %s\n", str($$));
}
             | package_name '.' identifier {
    MAYBE_ABORT;
    $$ = cons_str($3, strlen($3), $1);
} ;

package : PACKAGE package_name ';' {
    MAYBE_ABORT;
    char* pname = mapconcat(to_str, $2, ".");
    $$ = cons_str(pname, strlen(pname), nil);
    printf("package name concatenated: %s -> %s -> %s\n", str($2), pname, str($$));
    del($2);
} ;


syntax : SYNTAX '=' string_literal ';' { MAYBE_ABORT; $$ = NULL; } ;


user_type : IDENTIFIER {
    MAYBE_ABORT;
    $$ = cons_str($1, strlen($1), nil);
}
          | user_type '.' IDENTIFIER  {
    MAYBE_ABORT;
    $$ = cons_str($3, strlen($3), $1);
} ;


built_in_type : BOOL     { MAYBE_ABORT; $$ = "bool";       }
              | FIXED32  { MAYBE_ABORT; $$ = "fixed32";    }
              | FIXED64  { MAYBE_ABORT; $$ = "fixed64";    }
              | INT32    { MAYBE_ABORT; $$ = "int32";      }
              | INT64    { MAYBE_ABORT; $$ = "int64";      }
              | SFIXED32 { MAYBE_ABORT; $$ = "sfixed32";   }
              | SFIXED64 { MAYBE_ABORT; $$ = "sfixed64";   }
              | SINT32   { MAYBE_ABORT; $$ = "sint32";     }
              | SINT64   { MAYBE_ABORT; $$ = "sint64";     }
              | STRING   { MAYBE_ABORT; $$ = "string";     }
              | UINT32   { MAYBE_ABORT; $$ = "uint32";     }
              | UINT64   { MAYBE_ABORT; $$ = "uint64";     } ;


type : built_in_type {
     MAYBE_ABORT;
     $$ = cons_str($1, strlen($1), nil);
}
     | user_type { $$ = reverse($$); } ;


keyword : built_in_type
        | ENUM       { MAYBE_ABORT; $$ = "enum";       }
        | EXTENSIONS { MAYBE_ABORT; $$ = "extensions"; }
        | IMPORT     { MAYBE_ABORT; $$ = "import";     }
        | MESSAGE    { MAYBE_ABORT; $$ = "message";    }
        | MAP        { MAYBE_ABORT; $$ = "map";        }
        | ONEOF      { MAYBE_ABORT; $$ = "oneof";      }
        | OPTION     { MAYBE_ABORT; $$ = "option";     }
        | OPTIONAL   { MAYBE_ABORT; $$ = "optional";   }
        | PACKAGE    { MAYBE_ABORT; $$ = "package";    }
        | PUBLIC     { MAYBE_ABORT; $$ = "public";     }
        | REQUIRED   { MAYBE_ABORT; $$ = "required";   }
        | REPEATED   { MAYBE_ABORT; $$ = "repeated";   }
        | RESERVED   { MAYBE_ABORT; $$ = "reserved";   }
        | RETURNS    { MAYBE_ABORT; $$ = "returns";    }
        | RPC        { MAYBE_ABORT; $$ = "rpc";        }
        | SERVICE    { MAYBE_ABORT; $$ = "service";    }
        | STREAM     { MAYBE_ABORT; $$ = "stream";     }
        | SYNTAX     { MAYBE_ABORT; $$ = "syntax";     }
        | TO         { MAYBE_ABORT; $$ = "to";         }
        | WEAK       { MAYBE_ABORT; $$ = "weak";       }
        | MAX        { MAYBE_ABORT; $$ = "max";        } ;

identifier : keyword | IDENTIFIER ;


boolean : BOOL_TRUE { MAYBE_ABORT; $$ = from_ints(1, 1); }
        | BOOL_FALSE { MAYBE_ABORT; $$ = from_ints(1, 0); };


positive_int : POSINTEGER { MAYBE_ABORT; $$ = (size_t)atoi($1); } ;

literal : NEGINTEGER { MAYBE_ABORT; $$ = from_ints(1, atoi($1)); }
        | positive_int { MAYBE_ABORT; $$ = from_ints(1, $1); }
        | string_literal
        | boolean ;


assignment : identifier '=' literal { MAYBE_ABORT; $$ = NULL; } ;


option_def : assignment ';' { MAYBE_ABORT; $$ = NULL; } ;


field_options_body : assignment | field_options_body ',' assignment ;


field_options : '[' field_options_body ']' | %empty;


field_label : REQUIRED { MAYBE_ABORT; $$ = 1; }
            | REPEATED { MAYBE_ABORT; $$ = 2; }
            | OPTIONAL { MAYBE_ABORT; $$ = 3; }
            | %empty   { MAYBE_ABORT; $$ = 0; } ;


field : field_label type identifier '=' positive_int field_options ';' {
    MAYBE_ABORT;
    char* tname = mapconcat(to_str, $2, ".");
    list pos = from_ints(1, $5);
    list idf = cons_str($3, strlen($3), pos);
    $$ = tag(7, cons_str(tname, strlen(tname), idf));
    del($2);
} ;

oneof_field : type identifier '=' positive_int field_options ';' {
    MAYBE_ABORT;
    char* tname = mapconcat(to_str, $1, ".");
    list pos = from_ints(1, $4);
    list idf = cons_str($2, strlen($2), pos);
    $$ = tag(7, cons_str(tname, strlen(tname), idf));
    del($1);
} ;


oneof_fields : oneof_field {
    MAYBE_ABORT;
    $$ = cons($1, tlist, nil);
}            | oneof_fields oneof_field {
    MAYBE_ABORT;
    $$ = cons($2, tlist, $1);
} ;


oneof : ONEOF identifier '{' oneof_fields '}' {
    MAYBE_ABORT;
    $$ = tag(6, cons_str($2, strlen($2), $4));
} ;


key_type : INT32    { MAYBE_ABORT; $$ = 0;  }
         | INT64    { MAYBE_ABORT; $$ = 1;  }
         | UINT32   { MAYBE_ABORT; $$ = 2;  }
         | UINT64   { MAYBE_ABORT; $$ = 3;  }
         | SINT32   { MAYBE_ABORT; $$ = 4;  }
         | SINT64   { MAYBE_ABORT; $$ = 5;  }
         | FIXED32  { MAYBE_ABORT; $$ = 6;  }
         | FIXED64  { MAYBE_ABORT; $$ = 7;  }
         | SFIXED32 { MAYBE_ABORT; $$ = 8;  }
         | SFIXED64 { MAYBE_ABORT; $$ = 9;  }
         | BOOL     { MAYBE_ABORT; $$ = 10; }
         | STRING   { MAYBE_ABORT; $$ = 11; } ;


map_field : MAP '<' key_type ',' type '>' identifier '='
            positive_int field_options ';' { MAYBE_ABORT; $$ = NULL; } ;


range_end : MAX /* { $$ = SIZE_MAX; } */ { MAYBE_ABORT; $$ = NULL; }
          | positive_int { MAYBE_ABORT; $$ = NULL; } ;


range : positive_int TO range_end { MAYBE_ABORT; $$ = NULL; }
      | positive_int { MAYBE_ABORT; $$ = NULL; } ;


ranges : range | ranges ',' range { MAYBE_ABORT; $$ = NULL; } ;


reserved_strings : string_literal { MAYBE_ABORT; $$ = NULL; }
                 | reserved_strings ',' string_literal { MAYBE_ABORT; $$ = NULL; } ;


reserved_body : ranges | reserved_strings ;


reserved : RESERVED reserved_body { MAYBE_ABORT; $$ = NULL; } ;


option_name_parts : identifier { MAYBE_ABORT; $$ = cons_str($1, strlen($1), nil); }
                  | option_name_parts identifier {
    MAYBE_ABORT;
    $$ = cons_str($2, strlen($2), $1);
} ;


option_name : '(' option_name_parts ')' '.' option_name_parts {
    MAYBE_ABORT;
    $$ = cons($5, tlist, $2);
}
            | option_name_parts ;


enum_value_option : option_name '=' literal { MAYBE_ABORT; $$ = NULL; } ;


enum_value_options : enum_value_option
                   | enum_value_options ',' enum_value_option ;

enum_value_options_group : %empty { $$ = NULL; }
                         | '[' enum_value_options ']' { $$ = NULL; } ;


enum_field : identifier '=' positive_int enum_value_options_group {
    MAYBE_ABORT;
    $$ = cons_str($1, strlen($1), cons_int($3, 1, nil));
}
           | OPTION option_name '=' literal {
    MAYBE_ABORT;
    $$ = nil;
}
           | ';' { $$ = nil; } ;


enum_fields : enum_field {
    MAYBE_ABORT;
    if (!null($1)) {
        $$ = from_lists(1, $1);
    } else {
        $$ = nil;
    }
}
            | enum_fields enum_field {
    MAYBE_ABORT;
    if (!null($2)) {
        $$ = cons($2, tlist, $1);
    } else {
        $$ = $1;
    }
} ;


enum : ENUM identifier '{' enum_fields '}' {
    MAYBE_ABORT;
    $$ = cons_str($2, strlen($2), $4);
} ;


extensions : EXTENSIONS range { MAYBE_ABORT; $$ = NULL; } ;


message_field : enum              { MAYBE_ABORT; $$ = tag(1, $1); }
              | message           { MAYBE_ABORT; $$ = tag(0, $1); }
              | oneof
              | OPTION option_def { MAYBE_ABORT; $$ = nil; }
              | map_field         { MAYBE_ABORT; $$ = nil; }
              | field             { MAYBE_ABORT; $$ = $1; }
              | reserved          { MAYBE_ABORT; $$ = nil; }
              | extensions        { MAYBE_ABORT; $$ = nil; }
              | ';'               { MAYBE_ABORT; $$ = nil; } ;


message_block : message_field {
    MAYBE_ABORT;
    $$ = from_lists(1, $1);
}
              | message_block message_field {
    MAYBE_ABORT;
    $$ = cons($2, tlist, $1);
} ;


message : MESSAGE identifier '{' message_block '}' {
    MAYBE_ABORT;
    $$ = cons_str($2, strlen($2), $4);
} ;


rpc_type : STREAM type { MAYBE_ABORT; $$ = $2; }
         | type ;


rpc_options : '{' OPTION option_def '}' { MAYBE_ABORT; $$ = NULL; }
            | %empty { MAYBE_ABORT; $$ = NULL; } ;


rpc : identifier '(' rpc_type ')' RETURNS '(' rpc_type ')' rpc_options {
    MAYBE_ABORT;
    $$ = cons_str($1, strlen($1), nil);
} ;


service_body_part : OPTION option_def { MAYBE_ABORT; $$ = nil; }
                  | RPC rpc           { MAYBE_ABORT; $$ = $2; }
                  | ';'               { MAYBE_ABORT; $$ = nil; };



service_body : %empty { MAYBE_ABORT; $$ = from_lists(1, nil); }
             | service_body service_body_part {
    MAYBE_ABORT; 
    $$ = cons($2, tlist, $1);
} ;


service : SERVICE identifier '{' service_body '}' {
    MAYBE_ABORT;
    $$ = cons_str($2, strlen($2), $4);
} ;


top_level : message    { MAYBE_ABORT; $$ = tag(0, $1); }
          | enum       { MAYBE_ABORT; $$ = tag(1, $1); }
          | service    { MAYBE_ABORT; $$ = tag(3, $1); }
          | import     { MAYBE_ABORT; $$ = tag(4, $1); }
          | package    { MAYBE_ABORT; $$ = tag(5, $1); }
          | option_def { MAYBE_ABORT; $$ = nil; }
          | ';'        { MAYBE_ABORT; $$ = nil; } ;


top_levels : %empty {
    MAYBE_ABORT;
    $$ = from_lists(1, nil);
}
           | top_levels top_level {
    MAYBE_ABORT;
    $$ = cons($2, tlist, $1);
} ;


s : syntax top_levels {
    MAYBE_ABORT;
    $$ = $2;
    *result = $$;
} ;

%%
