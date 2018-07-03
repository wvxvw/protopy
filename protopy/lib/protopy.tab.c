/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */
#line 10 "protopy.y" /* yacc.c:339  */

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


#line 86 "protopy.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "protopy.tab.h".  */
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
    TBOOL = 258,
    BOOL_FALSE = 259,
    BOOL_TRUE = 260,
    ENUM = 261,
    EXTENSIONS = 262,
    EXTEND = 263,
    FIXED32 = 264,
    FIXED64 = 265,
    IMPORT = 266,
    TINT32 = 267,
    TINT64 = 268,
    MAP = 269,
    MAX = 270,
    MESSAGE = 271,
    ONEOF = 272,
    OPTION = 273,
    TOPTIONAL = 274,
    PACKAGE = 275,
    PUBLIC = 276,
    REPEATED = 277,
    REQUIRED = 278,
    RESERVED = 279,
    RETURNS = 280,
    RPC = 281,
    SERVICE = 282,
    SFIXED32 = 283,
    SFIXED64 = 284,
    SINT32 = 285,
    SINT64 = 286,
    STREAM = 287,
    STRING = 288,
    STRING_LITERAL = 289,
    SYNTAX = 290,
    TO = 291,
    TUINT32 = 292,
    TUINT64 = 293,
    WEAK = 294,
    POSINTEGER = 295,
    NEGINTEGER = 296,
    IDENTIFIER = 297
  };
#endif
/* Tokens.  */
#define TBOOL 258
#define BOOL_FALSE 259
#define BOOL_TRUE 260
#define ENUM 261
#define EXTENSIONS 262
#define EXTEND 263
#define FIXED32 264
#define FIXED64 265
#define IMPORT 266
#define TINT32 267
#define TINT64 268
#define MAP 269
#define MAX 270
#define MESSAGE 271
#define ONEOF 272
#define OPTION 273
#define TOPTIONAL 274
#define PACKAGE 275
#define PUBLIC 276
#define REPEATED 277
#define REQUIRED 278
#define RESERVED 279
#define RETURNS 280
#define RPC 281
#define SERVICE 282
#define SFIXED32 283
#define SFIXED64 284
#define SINT32 285
#define SINT64 286
#define STREAM 287
#define STRING 288
#define STRING_LITERAL 289
#define SYNTAX 290
#define TO 291
#define TUINT32 292
#define TUINT64 293
#define WEAK 294
#define POSINTEGER 295
#define NEGINTEGER 296
#define IDENTIFIER 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "protopy.y" /* yacc.c:355  */

    size_t keyword;
    int64_t index;
    char* string;
    list_t* object;
    void* nothing;

#line 218 "protopy.tab.c" /* yacc.c:355  */
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

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, void* scanner, list_t** result, apr_pool_t* mp);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);

#endif /* !YY_YY_PROTOPY_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 258 "protopy.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   756

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  163
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  262

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      54,    55,     2,     2,    49,     2,    44,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    43,
      52,    45,    53,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,     2,    48,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   105,   105,   107,   108,   109,   112,   115,   118,   122,
     129,   132,   135,   139,   142,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   159,   162,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   188,   188,   191,   192,   195,   197,   198,   199,   200,
     201,   203,   204,   206,   207,   209,   210,   213,   216,   219,
     219,   222,   222,   225,   226,   227,   228,   231,   248,   256,
     258,   263,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   282,   293,   294,   297,   298,   301,
     301,   304,   305,   308,   308,   311,   314,   314,   316,   319,
     323,   324,   327,   330,   333,   336,   337,   339,   340,   343,
     346,   349,   352,   359,   368,   373,   375,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   390,   397,   406,
     408,   413,   414,   417,   418,   419,   422,   427,   428,   429,
     433,   434,   439,   444,   445,   446,   447,   448,   449,   450,
     451,   454,   457,   466
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TBOOL", "BOOL_FALSE", "BOOL_TRUE",
  "ENUM", "EXTENSIONS", "EXTEND", "FIXED32", "FIXED64", "IMPORT", "TINT32",
  "TINT64", "MAP", "MAX", "MESSAGE", "ONEOF", "OPTION", "TOPTIONAL",
  "PACKAGE", "PUBLIC", "REPEATED", "REQUIRED", "RESERVED", "RETURNS",
  "RPC", "SERVICE", "SFIXED32", "SFIXED64", "SINT32", "SINT64", "STREAM",
  "STRING", "STRING_LITERAL", "SYNTAX", "TO", "TUINT32", "TUINT64", "WEAK",
  "POSINTEGER", "NEGINTEGER", "IDENTIFIER", "';'", "'.'", "'='", "':'",
  "'{'", "'}'", "','", "'['", "']'", "'<'", "'>'", "'('", "')'", "$accept",
  "string_literal", "import_kind", "import", "package_name", "package",
  "syntax", "user_type_tail", "user_type", "built_in_type", "type",
  "keyword", "identifier", "boolean", "positive_int", "literal",
  "option_kv", "option_kvs", "option_value", "assignment", "option_def",
  "field_options_body", "field_options", "field_label", "field",
  "oneof_field", "oneof_fields", "oneof", "key_type", "map_field",
  "range_end", "range", "ranges", "reserved_strings", "reserved_body",
  "reserved", "option_name_body_part", "option_name_body",
  "option_name_suffix", "option_name", "enum_value_option",
  "enum_value_options", "enum_value_options_group", "enum_field",
  "enum_fields", "enum", "extend", "extensions", "message_field",
  "message_block", "message", "rpc_type", "rpc_options", "rpc",
  "service_body_part", "service_body", "service", "top_level",
  "top_levels", "s", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,    59,    46,    61,    58,   123,   125,    44,
      91,    93,    60,    62,    40,    41
};
# endif

#define YYPACT_NINF -213

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-213)))

#define YYTABLE_NINF -53

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,    -8,  -213,    42,    18,    93,  -213,  -213,    26,   639,
       6,     2,   639,   329,   639,   639,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,     9,  -213,    35,    34,    33,  -213,  -213,    18,    36,
    -213,   676,  -213,    39,  -213,  -213,    44,    41,   -18,  -213,
      45,   601,    34,    47,   713,    53,   459,   -22,  -213,   676,
      13,  -213,   639,  -213,   329,  -213,    55,  -213,   421,  -213,
       4,    50,   639,   329,  -213,  -213,  -213,   -10,  -213,   561,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,   481,  -213,
    -213,  -213,   503,    59,  -213,  -213,  -213,  -213,  -213,   375,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,     3,    62,     4,
    -213,  -213,    74,  -213,   201,    68,  -213,  -213,  -213,    69,
      70,  -213,  -213,  -213,   639,  -213,  -213,  -213,   676,  -213,
      76,    77,    79,  -213,   237,    80,   329,   639,  -213,  -213,
    -213,    31,    82,     0,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,    81,   561,     4,    18,
      84,    44,    31,  -213,  -213,   375,  -213,    72,  -213,  -213,
     329,  -213,  -213,  -213,  -213,   561,   639,  -213,   164,  -213,
    -213,     4,  -213,   283,   525,    89,  -213,    10,    85,    90,
    -213,  -213,    91,  -213,   561,  -213,    87,    31,   329,  -213,
     639,     4,   329,    96,  -213,   115,  -213,  -213,    98,    91,
    -213,    19,  -213,    95,     4,   101,   329,  -213,   525,    91,
    -213,  -213,    97,   111,   110,  -213,     1,  -213,   329,  -213,
     114,  -213
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   161,     0,     0,   163,     1,     2,     0,     0,
       0,     5,     0,     0,     0,     0,   160,   156,   157,   154,
     159,   153,   155,   162,    10,    15,    30,    31,    16,    17,
      32,    18,    19,    34,    50,    33,    35,    36,    37,    38,
      39,    41,    40,    42,    43,    44,    45,    20,    21,    22,
      23,    46,    24,    47,    48,    25,    26,    49,    52,    29,
      51,     0,    11,     0,    14,     0,     4,     3,     0,     0,
     106,     0,   107,     0,   158,   108,   113,     0,     0,     7,
       0,     0,    13,     0,    76,     0,    76,     0,    68,     0,
       0,     9,     0,   150,    36,   121,     0,   122,     0,    12,
       0,     0,     0,     0,    75,    74,    73,     0,   136,     0,
     132,   129,   131,   133,   127,   135,   134,   137,    76,   128,
       6,   140,    76,   110,   109,    54,    53,    55,    56,     0,
      58,    60,    59,    57,    65,    67,     8,     0,     0,     0,
     124,   123,    98,   126,     0,     0,   130,   101,    99,   103,
     104,   105,    28,    27,     0,   125,   138,   139,     0,   112,
     106,   107,     0,    63,     0,     0,     0,     0,   149,   152,
     151,     0,   117,     0,    92,    88,    89,    82,    83,    90,
      91,    86,    87,    93,    84,    85,     0,     0,     0,     0,
       0,   111,     0,    66,    64,     0,   147,     0,   148,   120,
       0,   119,    95,    96,    97,     0,     0,    79,     0,   100,
     102,     0,    61,     0,     0,     0,   115,     0,     0,     0,
      81,    80,    72,    62,     0,   142,     0,     0,     0,   118,
       0,     0,     0,     0,   141,     0,   114,   116,     0,    72,
      69,     0,    77,     0,     0,     0,     0,    71,     0,    72,
      78,    70,     0,     0,   145,    94,     0,   146,     0,   144,
       0,   143
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,    -1,  -213,  -213,  -213,  -213,  -213,   100,    -6,  -100,
     -93,   -13,    -7,  -213,   -94,  -161,  -150,   -37,  -213,  -212,
    -102,  -213,  -211,  -213,  -213,   -49,  -213,  -213,  -213,  -213,
    -213,   -95,  -213,  -213,  -213,  -213,    75,   -60,  -213,   -67,
     -62,  -213,  -213,    71,  -213,   163,   165,  -213,   -79,    86,
     166,   -73,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   130,    68,    17,    78,    18,     2,    64,   152,    59,
     206,    60,   162,   132,   133,   134,   163,   164,   135,    73,
      74,   241,   233,   109,   110,   207,   208,   111,   186,   112,
     204,   143,   149,   150,   151,   113,    75,    76,   159,    77,
     216,   217,   201,    97,    98,   114,   115,   116,   117,   118,
     119,   226,   257,   198,   170,   137,    22,    23,     5,     3
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,   146,    61,     8,    65,    69,   142,    79,    80,   153,
     199,    87,   148,   142,   194,   202,   154,   125,   126,   258,
     240,   166,    89,    66,     7,    91,    92,   138,   245,   167,
     127,   212,     1,   123,   251,   125,   126,     4,   253,   156,
     127,    67,     6,   156,   127,   172,   168,     7,    62,   259,
      63,   169,     7,   127,   128,    62,    81,    63,    72,   228,
     129,   229,   165,   194,   196,     7,   236,    85,   246,    24,
     247,   127,   128,    62,    96,    63,    72,    62,    83,   203,
      84,    72,    88,    86,   131,   136,    90,   153,    89,    99,
      72,    96,    93,   209,   142,   145,   120,   165,   191,     9,
     139,    10,   144,   158,    11,   153,   147,   171,   153,    12,
     173,    13,   218,    14,   153,   187,   161,   222,   188,   189,
      15,   225,   -52,   -51,   153,   192,   214,   195,   165,   211,
     205,   234,   200,   215,   227,   231,    16,   239,   230,   242,
     243,   232,   235,   244,   250,    72,   165,   190,   153,   248,
     249,   161,   254,    72,   255,   225,   260,   256,   213,   221,
     197,   215,   261,    82,   124,   131,   237,    25,    19,   141,
      20,    21,   122,    28,    29,   252,    31,    32,     0,     0,
       0,     0,   161,     0,     0,     0,   131,    72,   210,     0,
       0,     0,    47,    48,    49,    50,     0,    52,     0,   219,
     161,    55,    56,     0,   174,     0,    62,     0,    63,     0,
     175,   176,   220,   177,   178,    72,     0,     0,     0,    72,
       0,   131,     0,   238,     0,     0,     0,     0,     0,   179,
     180,   181,   182,    72,   183,     0,     0,     0,   184,   185,
      25,     0,     0,    26,    27,    72,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,    53,    54,    55,    56,    57,     0,     0,   160,
       0,     0,     0,     0,     0,   193,    25,     0,     0,    26,
      27,    71,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,    53,    54,
      55,    56,    57,     0,     0,   160,     0,     0,     0,     0,
       0,   223,    25,     0,     0,    26,    27,    71,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,    53,    54,    55,    56,    57,     0,
       0,    70,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    26,    27,    71,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,     0,
      53,    54,    55,    56,    57,     0,     0,   160,     0,     0,
       0,     0,     0,     0,    25,     0,     0,    26,    27,    71,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    94,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,    53,    54,    55,    56,
      57,     0,     0,    58,    95,     9,   100,    10,     0,   140,
       0,     0,     0,   101,     0,    12,   102,   103,   104,     0,
       0,   105,   106,   107,     0,     0,     0,     9,   100,    10,
       0,     0,     0,     0,     0,   101,     0,    12,   102,   103,
     104,     0,   108,   105,   106,   107,     0,   121,     0,     9,
     100,    10,     0,     0,     0,     0,     0,   101,     0,    12,
     102,   103,   104,     0,   108,   105,   106,   107,    25,   155,
       0,     0,     0,     0,    28,    29,     0,    31,    32,     0,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
       0,   157,     0,    47,    48,    49,    50,   224,    52,     0,
       0,     0,    55,    56,    25,     0,     0,    62,     0,    63,
      28,    29,     0,    31,    32,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,    50,     0,    52,     0,     0,     0,    55,    56,
       0,     0,     0,    62,    25,    63,     0,    26,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    94,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,    53,    54,    55,    56,
      57,     0,    25,    58,    95,    26,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,    53,    54,    55,    56,    57,    25,
       0,    58,    26,    27,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       0,    53,    54,    55,    56,    57,     0,     0,    70,     9,
     100,    10,     0,     0,     0,     0,     0,   101,     0,    12,
     102,   103,   104,     0,     0,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108
};

static const yytype_int16 yycheck[] =
{
      13,   103,     9,     4,    10,    12,   100,    14,    15,   109,
     171,    71,   107,   107,   164,    15,   109,     4,     5,    18,
     232,    18,    44,    21,    34,    43,    44,    94,   239,    26,
      40,   192,    35,    55,   246,     4,     5,    45,   249,   118,
      40,    39,     0,   122,    40,   139,    43,    34,    42,    48,
      44,    48,    34,    40,    41,    42,    47,    44,    71,    49,
      47,    51,   129,   213,   166,    34,   227,    68,    49,    43,
      51,    40,    41,    42,    81,    44,    89,    42,    44,   173,
      47,    94,    43,    47,    90,    92,    45,   187,    44,    42,
     103,    98,    47,   188,   188,   102,    43,   164,   158,     6,
      45,     8,    52,    44,    11,   205,   107,    45,   208,    16,
      36,    18,   205,    20,   214,    47,   129,   211,    49,    49,
      27,   214,    46,    46,   224,    46,    54,    47,   195,    45,
      49,   224,    50,   200,    45,    45,    43,   231,    53,    43,
      25,    50,    55,    45,    43,   158,   213,   154,   248,    54,
     244,   164,    55,   166,    43,   248,   258,    47,   195,   208,
     167,   228,    48,    63,    89,   171,   228,     3,     5,    98,
       5,     5,    86,     9,    10,   248,    12,    13,    -1,    -1,
      -1,    -1,   195,    -1,    -1,    -1,   192,   200,   189,    -1,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    -1,   206,
     213,    37,    38,    -1,     3,    -1,    42,    -1,    44,    -1,
       9,    10,    48,    12,    13,   228,    -1,    -1,    -1,   232,
      -1,   227,    -1,   230,    -1,    -1,    -1,    -1,    -1,    28,
      29,    30,    31,   246,    33,    -1,    -1,    -1,    37,    38,
       3,    -1,    -1,     6,     7,   258,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,     3,    -1,    -1,     6,
       7,    54,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,     3,    -1,    -1,     6,     7,    54,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,     6,     7,    54,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,     6,     7,    54,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    -1,    -1,    42,    43,     6,     7,     8,    -1,    48,
      -1,    -1,    -1,    14,    -1,    16,    17,    18,    19,    -1,
      -1,    22,    23,    24,    -1,    -1,    -1,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    17,    18,
      19,    -1,    43,    22,    23,    24,    -1,    48,    -1,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      17,    18,    19,    -1,    43,    22,    23,    24,     3,    48,
      -1,    -1,    -1,    -1,     9,    10,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    48,    -1,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,     3,    -1,    -1,    42,    -1,    44,
       9,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    37,    38,
      -1,    -1,    -1,    42,     3,    44,    -1,     6,     7,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    -1,     3,    42,    43,     6,     7,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,     3,
      -1,    42,     6,     7,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    39,    -1,    -1,    42,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      17,    18,    19,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    35,    62,   115,    45,   114,     0,    34,    57,     6,
       8,    11,    16,    18,    20,    27,    43,    59,    61,   101,
     102,   106,   112,   113,    43,     3,     6,     7,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    35,    36,    37,    38,    39,    42,    65,
      67,    68,    42,    44,    63,    64,    21,    39,    58,    68,
      42,    54,    67,    75,    76,    92,    93,    95,    60,    68,
      68,    47,    63,    44,    47,    57,    47,    93,    43,    44,
      45,    43,    44,    47,    18,    43,    68,    99,   100,    42,
       7,    14,    17,    18,    19,    22,    23,    24,    43,    79,
      80,    83,    85,    91,   101,   102,   103,   104,   105,   106,
      43,    48,   105,    55,    92,     4,     5,    40,    41,    47,
      57,    64,    69,    70,    71,    74,    68,   111,    95,    45,
      48,    99,    70,    87,    52,    68,    76,    57,    87,    88,
      89,    90,    64,    65,    66,    48,   104,    48,    44,    94,
      42,    67,    68,    72,    73,    95,    18,    26,    43,    48,
     110,    45,    70,    36,     3,     9,    10,    12,    13,    28,
      29,    30,    31,    33,    37,    38,    84,    47,    49,    49,
      68,    93,    46,    48,    72,    47,    76,    68,   109,    71,
      50,    98,    15,    70,    86,    49,    66,    81,    82,    87,
      57,    45,    71,    73,    54,    95,    96,    97,    66,    68,
      48,    81,    70,    48,    32,    66,   107,    45,    49,    51,
      53,    45,    50,    78,    66,    55,    71,    96,    68,    70,
      75,    77,    43,    25,    45,    78,    49,    51,    54,    70,
      43,    75,   107,    78,    55,    43,    47,   108,    18,    48,
      76,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    58,    59,    60,    60,    61,
      62,    63,    63,    64,    64,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    66,    66,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    69,    69,    70,    71,    71,    71,    71,
      71,    72,    72,    73,    73,    74,    74,    75,    76,    77,
      77,    78,    78,    79,    79,    79,    79,    80,    81,    82,
      82,    83,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    85,    86,    86,    87,    87,    88,
      88,    89,    89,    90,    90,    91,    92,    92,    93,    93,
      94,    94,    95,    95,    96,    97,    97,    98,    98,    99,
      99,    99,   100,   100,   101,   102,   103,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   105,   105,   106,
     106,   107,   107,   108,   108,   108,   109,   110,   110,   110,
     111,   111,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   115
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     0,     4,     1,     3,     3,
       4,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     1,     2,     1,     3,     3,     2,     1,
       3,     3,     0,     1,     1,     1,     0,     7,     6,     1,
       2,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,    11,     1,     1,     3,     1,     1,
       3,     1,     3,     1,     1,     2,     1,     1,     1,     3,
       0,     2,     4,     1,     3,     1,     3,     0,     3,     4,
       4,     1,     1,     2,     5,     5,     2,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     5,
       4,     2,     1,     4,     2,     0,     9,     2,     2,     1,
       0,     2,     5,     1,     1,     1,     1,     1,     2,     1,
       1,     0,     2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, scanner, result, mp, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner, result, mp); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, list_t** result, apr_pool_t* mp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (result);
  YYUSE (mp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, list_t** result, apr_pool_t* mp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, result, mp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void* scanner, list_t** result, apr_pool_t* mp)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, result, mp);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, result, mp); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void* scanner, list_t** result, apr_pool_t* mp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (result);
  YYUSE (mp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}



struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;
    /* Used to determine if this is the first time this instance has
       been used.  */
    int yynew;
  };

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = (yypstate *) malloc (sizeof *yyps);
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
#ifndef yyoverflow
  /* If the stack was reallocated but the parse did not complete, then the
     stack still needs to be freed.  */
  if (!yyps->yynew && yyps->yyss != yyps->yyssa)
    YYSTACK_FREE (yyps->yyss);
#endif
  free (yyps);
}

#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yyerror_range yyps->yyerror_range
#define yystacksize yyps->yystacksize


/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, void* scanner, list_t** result, apr_pool_t* mp)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  if (!yyps->yynew)
    {
      yyn = yypact[yystate];
      goto yyread_pushed_token;
    }

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = *yypushed_loc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 105 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), nil, mp); }
#line 1784 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 107 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 1; }
#line 1790 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 108 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 2; }
#line 1796 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 109 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 0; }
#line 1802 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 112 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[-1].object); }
#line 1808 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 115 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), nil, mp);
}
#line 1816 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 118 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), (yyvsp[-2].object), mp);
}
#line 1824 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 122 "protopy.y" /* yacc.c:1646  */
    {
    (yyvsp[-1].object) = nreverse((yyvsp[-1].object));
    char* pname = mapconcat(to_str, (yyvsp[-1].object), ".", mp);
    (yyval.object) = cons_str(pname, strlen(pname), nil, mp);
}
#line 1834 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 129 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 1840 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 132 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), nil, mp);
}
#line 1848 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 135 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), (yyvsp[-2].object), mp);
}
#line 1856 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 139 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = nappend((yyvsp[0].object), cons_str(".", 1, nil, mp));
}
#line 1864 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 145 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "bool";       }
#line 1870 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 146 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "fixed32";    }
#line 1876 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 147 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "fixed64";    }
#line 1882 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 148 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "int32";      }
#line 1888 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 149 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "int64";      }
#line 1894 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 150 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "sfixed32";   }
#line 1900 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 151 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "sfixed64";   }
#line 1906 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 152 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "sint32";     }
#line 1912 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 153 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "sint64";     }
#line 1918 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 154 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "string";     }
#line 1924 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 155 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "uint32";     }
#line 1930 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 156 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "uint64";     }
#line 1936 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 159 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), nil, mp);
}
#line 1944 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 162 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = reverse((yyval.object), mp); }
#line 1950 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 166 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "enum";       }
#line 1956 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 167 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "extensions"; }
#line 1962 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 168 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "import";     }
#line 1968 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 169 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "message";    }
#line 1974 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 170 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "map";        }
#line 1980 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 171 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "oneof";      }
#line 1986 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 172 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "option";     }
#line 1992 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 173 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "optional";   }
#line 1998 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 174 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "package";    }
#line 2004 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 175 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "public";     }
#line 2010 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 176 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "required";   }
#line 2016 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 177 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "repeated";   }
#line 2022 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 178 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "reserved";   }
#line 2028 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 179 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "returns";    }
#line 2034 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 180 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "rpc";        }
#line 2040 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 181 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "service";    }
#line 2046 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 182 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "stream";     }
#line 2052 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 183 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "syntax";     }
#line 2058 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 184 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "to";         }
#line 2064 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 185 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "weak";       }
#line 2070 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 186 "protopy.y" /* yacc.c:1646  */
    { (yyval.string) = "max";        }
#line 2076 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 191 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = cons_int(1, sizeof(int), nil, mp); }
#line 2082 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 192 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = cons_int(0, sizeof(int), nil, mp); }
#line 2088 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 195 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = (size_t)atoi((yyvsp[0].string)); }
#line 2094 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 197 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = cons_int(atoi((yyvsp[0].string)), sizeof(int), nil, mp); }
#line 2100 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 198 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = cons_int((yyvsp[0].index), sizeof(int), nil, mp); }
#line 2106 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 203 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2112 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 204 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2118 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 209 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2124 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 210 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2130 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 213 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2136 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 216 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2142 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 225 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 1; }
#line 2148 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 226 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 2; }
#line 2154 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 227 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 3; }
#line 2160 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 228 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = 0; }
#line 2166 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 231 "protopy.y" /* yacc.c:1646  */
    {
    char* tname = mapconcat(to_str, (yyvsp[-5].object), ".", mp);
    list_t* pos = cons_int((yyvsp[-2].index), sizeof(int), nil, mp);
    list_t* idf = cons_str((yyvsp[-4].string), strlen((yyvsp[-4].string)), pos, mp);
    int ftag;

    switch ((yyvsp[-6].index)) {
        case 2:
            ftag = 8;
            break;
        default:
            ftag = 7;
            break;
    }
    (yyval.object) = tag(ftag, cons_str(tname, strlen(tname), idf, mp), mp);
}
#line 2187 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 248 "protopy.y" /* yacc.c:1646  */
    {
    char* tname = mapconcat(to_str, (yyvsp[-5].object), ".", mp);
    list_t* pos = cons_int((yyvsp[-2].index), sizeof(int), nil, mp);
    list_t* idf = cons_str((yyvsp[-4].string), strlen((yyvsp[-4].string)), pos, mp);
    (yyval.object) = tag(7, cons_str(tname, strlen(tname), idf, mp), mp);
}
#line 2198 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 256 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons((yyvsp[0].object), tlist, nil, mp);
}
#line 2206 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 258 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object), mp);
}
#line 2214 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 263 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = tag(6, cons_str((yyvsp[-3].string), strlen((yyvsp[-3].string)), (yyvsp[-1].object), mp), mp);
}
#line 2222 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 268 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_int32;    }
#line 2228 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 269 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_int64;    }
#line 2234 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 270 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_uint32;   }
#line 2240 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 271 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_uint64;   }
#line 2246 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 272 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_sint32;   }
#line 2252 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 273 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_sint64;   }
#line 2258 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 274 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_fixed32;  }
#line 2264 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 275 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_fixed64;  }
#line 2270 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 276 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_sfixed32; }
#line 2276 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 277 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_sfixed64; }
#line 2282 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 278 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_bool;     }
#line 2288 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 279 "protopy.y" /* yacc.c:1646  */
    { (yyval.index) = vt_string;   }
#line 2294 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 283 "protopy.y" /* yacc.c:1646  */
    {
    char* vtype = mapconcat(to_str, (yyvsp[-6].object), ".", mp);
    (yyval.object) = tag(9, cons(
                cons_int((yyvsp[-8].index), 1, cons_str(vtype, strlen(vtype), nil, mp), mp),
                tlist,
                cons_str((yyvsp[-4].string), strlen((yyvsp[-4].string)), cons_int((yyvsp[-2].index), 1, nil, mp), mp), mp),
                mp);
}
#line 2307 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 293 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2313 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 294 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2319 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 297 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2325 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 298 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2331 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 301 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2337 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 304 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2343 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 305 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2349 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 311 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2355 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 316 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[0].string), strlen((yyvsp[0].string)), nil, mp);
}
#line 2363 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 319 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons((yyvsp[0].string), tstr, (yyvsp[-2].object), mp);
}
#line 2371 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 323 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2377 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 324 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[0].object); }
#line 2383 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 327 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-2].object), mp);
}
#line 2391 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 333 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2397 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 339 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2403 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 340 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2409 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 343 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-3].string), strlen((yyvsp[-3].string)), cons_int((yyvsp[-1].index), 1, nil, mp), mp);
}
#line 2417 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 346 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = nil;
}
#line 2425 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 349 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2431 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 352 "protopy.y" /* yacc.c:1646  */
    {
    if (!null((yyvsp[0].object))) {
        (yyval.object) = from_lists(1, mp, (yyvsp[0].object));
    } else {
        (yyval.object) = nil;
    }
}
#line 2443 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 359 "protopy.y" /* yacc.c:1646  */
    {
    if (!null((yyvsp[0].object))) {
        (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object), mp);
    } else {
        (yyval.object) = (yyvsp[-1].object);
    }
}
#line 2455 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 368 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-3].string), strlen((yyvsp[-3].string)), (yyvsp[-1].object), mp);
}
#line 2463 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 373 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2469 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 375 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2475 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 378 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(1, (yyvsp[0].object), mp); }
#line 2481 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 379 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(0, (yyvsp[0].object), mp); }
#line 2487 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 381 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2493 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 382 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[0].object); }
#line 2499 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 383 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[0].object); }
#line 2505 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 384 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2511 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 385 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2517 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 386 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2523 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 387 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2529 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 390 "protopy.y" /* yacc.c:1646  */
    {
    if (null((yyvsp[0].object))) {
        (yyval.object) = nil;
    } else {
        (yyval.object) = from_lists(1, mp, (yyvsp[0].object));
    }
}
#line 2541 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 397 "protopy.y" /* yacc.c:1646  */
    {
    if (null((yyvsp[0].object))) {
        (yyval.object) = (yyvsp[-1].object);
    } else {
        (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object), mp);
    }
}
#line 2553 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 406 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-3].string), strlen((yyvsp[-3].string)), (yyvsp[-1].object), mp);
}
#line 2561 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 408 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-2].string), strlen((yyvsp[-2].string)), nil, mp);
}
#line 2569 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 413 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[0].object); }
#line 2575 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 417 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2581 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 418 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2587 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 419 "protopy.y" /* yacc.c:1646  */
    { (yyval.nothing) = NULL; }
#line 2593 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 422 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-8].string), strlen((yyvsp[-8].string)), nil, mp);
}
#line 2601 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 427 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2607 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 428 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = (yyvsp[0].object); }
#line 2613 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 429 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2619 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 433 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = from_lists(1, mp, nil); }
#line 2625 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 434 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object), mp);
}
#line 2633 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 439 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = cons_str((yyvsp[-3].string), strlen((yyvsp[-3].string)), (yyvsp[-1].object), mp);
}
#line 2641 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 444 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(0, (yyvsp[0].object), mp); }
#line 2647 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 445 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(1, (yyvsp[0].object), mp); }
#line 2653 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 446 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(3, (yyvsp[0].object), mp); }
#line 2659 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 447 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(4, (yyvsp[0].object), mp); }
#line 2665 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 448 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = tag(5, (yyvsp[0].object), mp); }
#line 2671 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 449 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2677 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 450 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2683 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 451 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = nil; }
#line 2689 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 454 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = from_lists(1, mp, nil);
}
#line 2697 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 457 "protopy.y" /* yacc.c:1646  */
    {
    if (null((yyvsp[0].object))) {
        (yyval.object) = (yyvsp[-1].object);
    } else {
        (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object), mp);
    }
}
#line 2709 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 466 "protopy.y" /* yacc.c:1646  */
    {
    (yyval.object) = (yyvsp[0].object);
    *result = (yyval.object);
}
#line 2718 "protopy.tab.c" /* yacc.c:1646  */
    break;


#line 2722 "protopy.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, result, mp, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, result, mp, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner, result, mp);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner, result, mp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, result, mp, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, result, mp);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, result, mp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yyps->yynew = 1;

yypushreturn:
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 471 "protopy.y" /* yacc.c:1906  */

