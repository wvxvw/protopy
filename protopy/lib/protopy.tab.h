/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PROTOPY_TAB_H_INCLUDED
# define YY_YY_PROTOPY_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BOOL = 258,
    BOOL_FALSE = 259,
    BOOL_TRUE = 260,
    ENUM = 261,
    EXTENSIONS = 262,
    FIXED32 = 263,
    FIXED64 = 264,
    IMPORT = 265,
    INT32 = 266,
    INT64 = 267,
    MAP = 268,
    MAX = 269,
    MESSAGE = 270,
    ONEOF = 271,
    OPTION = 272,
    OPTIONAL = 273,
    PACKAGE = 274,
    PUBLIC = 275,
    REPEATED = 276,
    REQUIRED = 277,
    RESERVED = 278,
    RETURNS = 279,
    RPC = 280,
    SERVICE = 281,
    SFIXED32 = 282,
    SFIXED64 = 283,
    SINT32 = 284,
    SINT64 = 285,
    STREAM = 286,
    STRING = 287,
    STRING_LITERAL = 288,
    SYNTAX = 289,
    TO = 290,
    UINT32 = 291,
    UINT64 = 292,
    WEAK = 293,
    POSINTEGER = 294,
    NEGINTEGER = 295,
    IDENTIFIER = 296
  };
#endif
/* Tokens.  */
#define BOOL 258
#define BOOL_FALSE 259
#define BOOL_TRUE 260
#define ENUM 261
#define EXTENSIONS 262
#define FIXED32 263
#define FIXED64 264
#define IMPORT 265
#define INT32 266
#define INT64 267
#define MAP 268
#define MAX 269
#define MESSAGE 270
#define ONEOF 271
#define OPTION 272
#define OPTIONAL 273
#define PACKAGE 274
#define PUBLIC 275
#define REPEATED 276
#define REQUIRED 277
#define RESERVED 278
#define RETURNS 279
#define RPC 280
#define SERVICE 281
#define SFIXED32 282
#define SFIXED64 283
#define SINT32 284
#define SINT64 285
#define STREAM 286
#define STRING 287
#define STRING_LITERAL 288
#define SYNTAX 289
#define TO 290
#define UINT32 291
#define UINT64 292
#define WEAK 293
#define POSINTEGER 294
#define NEGINTEGER 295
#define IDENTIFIER 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 35 "protopy.y" /* yacc.c:1909  */

    size_t keyword;
    int64_t index;
    char* string;
    list object;
    void* nothing;

#line 144 "protopy.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, void* scanner, list* result);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);

#endif /* !YY_YY_PROTOPY_TAB_H_INCLUDED  */
