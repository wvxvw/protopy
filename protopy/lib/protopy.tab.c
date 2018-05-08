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
#line 9 "protopy.y" /* yacc.c:339  */

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
    int* val = malloc(sizeof(int));
    *val = t;
    return cons(val, tint, body);
}

#define MAYBE_ABORT                             \
do {                                            \
    if (false) {                                \
        YYABORT;                                \
    }                                           \
} while (0)

#line 94 "protopy.tab.c" /* yacc.c:339  */

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
#line 37 "protopy.y" /* yacc.c:355  */

    size_t keyword;
    int64_t index;
    char* string;
    list object;
    void* nothing;

#line 224 "protopy.tab.c" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */

#line 264 "protopy.tab.c" /* yacc.c:358  */

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
#define YYLAST   457

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  230

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

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
      52,    53,     2,     2,    45,     2,    43,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
      50,    44,    51,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    46,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   109,   109,   111,   112,   113,   116,   119,   120,   125,
     132,   135,   136,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   156,   157,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   183,
     183,   186,   187,   190,   192,   193,   194,   195,   198,   201,
     204,   204,   207,   207,   210,   211,   212,   213,   216,   224,
     233,   236,   242,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   259,   263,   264,   267,   268,
     271,   271,   274,   275,   278,   278,   281,   284,   285,   291,
     295,   298,   301,   302,   305,   309,   310,   313,   314,   317,
     323,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     337,   341,   347,   353,   354,   357,   358,   361,   367,   368,
     369,   373,   374,   380,   386,   387,   388,   389,   390,   391,
     392,   395,   399,   405
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BOOL", "BOOL_FALSE", "BOOL_TRUE",
  "ENUM", "EXTENSIONS", "FIXED32", "FIXED64", "IMPORT", "INT32", "INT64",
  "MAP", "MAX", "MESSAGE", "ONEOF", "OPTION", "OPTIONAL", "PACKAGE",
  "PUBLIC", "REPEATED", "REQUIRED", "RESERVED", "RETURNS", "RPC",
  "SERVICE", "SFIXED32", "SFIXED64", "SINT32", "SINT64", "STREAM",
  "STRING", "STRING_LITERAL", "SYNTAX", "TO", "UINT32", "UINT64", "WEAK",
  "POSINTEGER", "NEGINTEGER", "IDENTIFIER", "';'", "'.'", "'='", "','",
  "'['", "']'", "'{'", "'}'", "'<'", "'>'", "'('", "')'", "$accept",
  "string_literal", "import_kind", "import", "package_name", "package",
  "syntax", "user_type", "built_in_type", "type", "keyword", "identifier",
  "boolean", "positive_int", "literal", "assignment", "option_def",
  "field_options_body", "field_options", "field_label", "field",
  "oneof_field", "oneof_fields", "oneof", "key_type", "map_field",
  "range_end", "range", "ranges", "reserved_strings", "reserved_body",
  "reserved", "option_name_parts", "option_name", "enum_value_option",
  "enum_value_options", "enum_field", "enum_fields", "enum", "extensions",
  "message_field", "message_block", "message", "rpc_type", "rpc_options",
  "rpc", "service_body_part", "service_body", "service", "top_level",
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
     295,   296,    59,    46,    61,    44,    91,    93,   123,   125,
      60,    62,    40,    41
};
# endif

#define YYPACT_NINF -201

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-201)))

#define YYTABLE_NINF -31

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -22,   -15,  -201,    50,    22,   280,  -201,  -201,    24,  -201,
     317,  -201,  -201,  -201,    -3,  -201,  -201,  -201,  -201,   317,
    -201,  -201,  -201,   317,  -201,  -201,  -201,  -201,  -201,  -201,
     317,  -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,
    -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,    27,    26,
    -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,
    -201,    34,  -201,  -201,    22,    41,    45,  -201,    42,    89,
    -201,   353,    32,    31,  -201,   317,  -201,  -201,  -201,  -201,
    -201,  -201,  -201,  -201,  -201,   233,    35,  -201,    -9,  -201,
     317,    52,    47,   317,   317,   317,  -201,  -201,  -201,     6,
    -201,   142,  -201,  -201,  -201,  -201,  -201,  -201,  -201,     9,
    -201,  -201,    -6,   317,  -201,   317,    55,    52,   353,  -201,
      65,  -201,   420,    53,  -201,  -201,  -201,    57,    59,  -201,
    -201,    63,  -201,   317,  -201,  -201,   317,   317,  -201,  -201,
    -201,   185,  -201,    89,    61,  -201,    20,  -201,  -201,  -201,
    -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,  -201,    64,
     142,    52,    22,    71,    69,  -201,    62,  -201,    74,  -201,
     233,  -201,  -201,  -201,   142,   317,  -201,   107,  -201,  -201,
    -201,    52,   389,   317,    81,  -201,   -24,    75,    86,  -201,
    -201,    92,   142,  -201,    79,   317,    89,   233,  -201,   317,
      52,   317,    98,  -201,   117,  -201,  -201,   102,    92,  -201,
      15,  -201,    90,    52,   105,   317,  -201,   389,    92,  -201,
    -201,    96,   110,   109,  -201,   138,  -201,   317,   113,  -201
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   141,     0,     0,   143,     1,     2,     0,    13,
      28,    29,    14,    15,     5,    16,    17,    32,    48,    31,
      33,    34,    35,    36,    37,    39,    38,    40,    41,    42,
      43,    18,    19,    20,    21,    44,    22,    45,    46,    23,
      24,    47,    50,   140,   137,   138,    27,    49,     0,     0,
     139,   135,   134,   136,   142,    10,    28,    30,    31,    36,
      43,     0,     4,     3,     0,     0,     0,     7,     0,     0,
      59,     0,     0,    67,     9,     0,   131,    52,    51,    53,
      54,    56,    57,    55,    58,    34,     0,   107,     0,     6,
       0,     0,     0,     0,     0,     0,    66,    65,    64,     0,
     119,     0,   116,   113,   115,   117,   111,   118,   120,    67,
     112,     8,     0,     0,    97,   100,     0,     0,     0,   109,
      89,   110,     0,     0,   114,    92,    90,    94,    95,    96,
      11,    26,    25,     0,   122,   121,     0,     0,   130,   133,
     132,     0,    98,     0,   105,   108,     0,    83,    79,    80,
      73,    74,    81,    82,    77,    78,    84,    75,    76,     0,
       0,     0,     0,     0,     0,   128,     0,   129,     0,   106,
       0,    86,    87,    88,     0,     0,    70,     0,    91,    93,
      12,     0,     0,     0,     0,   102,     0,     0,     0,    72,
      71,    63,     0,   124,     0,    99,     0,     0,   104,     0,
       0,     0,     0,   123,     0,   101,   103,     0,    63,    60,
       0,    68,     0,     0,     0,     0,    62,     0,    63,    69,
      61,     0,     0,   126,    85,     0,   127,     0,     0,   125
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -201,    -1,  -201,  -201,  -201,  -201,  -201,  -201,   -96,   -97,
    -201,   -10,  -201,   -89,  -129,  -145,   -94,  -201,  -200,  -201,
    -201,   -19,  -201,  -201,  -201,  -201,  -201,   -92,  -201,  -201,
    -201,  -201,  -107,    78,   -38,  -201,    46,  -201,   161,  -201,
      58,  -201,   163,   -42,  -201,  -201,  -201,  -201,  -201,  -201,
    -201,  -201
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    81,    64,    44,    66,    45,     2,   131,    46,   175,
      47,    48,    82,    83,    84,    49,    50,   210,   202,   101,
     102,   176,   177,   103,   159,   104,   173,   121,   127,   128,
     129,   105,   115,   184,   185,   186,    87,    88,   106,   107,
     108,   109,   110,   194,   226,   167,   140,   112,    53,    54,
       5,     3
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,   124,   120,     8,   133,   132,   141,   126,   214,    65,
     120,   136,     1,    67,   169,    90,    91,    62,   222,   137,
      68,   197,    92,   198,    93,    94,    95,    96,   144,     4,
      97,    98,    99,   118,   171,    63,   138,    90,    91,     7,
     119,   -30,   165,   139,    92,    79,    93,    94,    95,    96,
       6,   100,    97,    98,    99,     7,   209,   172,   134,    79,
     215,    86,   216,    72,   132,   111,    55,   205,    70,   178,
     220,    69,   120,   100,    89,   114,   195,   187,   132,   117,
      61,   132,    71,    65,   123,   193,   132,    74,    75,    73,
      76,    79,   191,    77,    78,   203,   132,   122,   125,   143,
     146,   160,   161,   114,   162,   142,   163,   170,    86,   174,
       9,   208,   180,   181,   182,    12,    13,   183,    15,    16,
     193,   132,     7,   164,   218,   196,   199,   166,    79,    80,
     200,   142,   204,   228,    31,    32,    33,    34,   201,    36,
     211,   212,   217,    39,    40,     9,   213,   219,   130,   223,
      12,    13,   224,    15,    16,   227,   189,   225,   190,   206,
     114,   179,   229,   116,   145,   188,    51,   135,    52,    31,
      32,    33,    34,   114,    36,   221,     0,     0,    39,    40,
       0,     0,     0,   130,     0,   142,     0,   114,     9,   207,
       0,    56,    11,    12,    13,    57,    15,    16,    17,    18,
      58,    20,    21,    22,    59,    24,    25,    26,    27,    28,
      29,    60,    31,    32,    33,    34,    35,    36,     0,    37,
      38,    39,    40,    41,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     9,     0,   168,    56,
      11,    12,    13,    57,    15,    16,    17,    18,    58,    20,
      21,    22,    59,    24,    25,    26,    27,    28,    29,    60,
      31,    32,    33,    34,    35,    36,     0,    37,    38,    39,
      40,    41,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,   113,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,     0,    37,    38,    39,    40,    41,     0,
       9,    42,    43,    56,    11,    12,    13,    57,    15,    16,
      17,    18,    58,    20,    21,    22,    59,    24,    25,    26,
      27,    28,    29,    60,    31,    32,    33,    34,    35,    36,
       0,    37,    38,    39,    40,    41,     9,     0,    42,    56,
      11,    12,    13,    57,    15,    16,    17,    18,    58,    20,
      85,    22,    59,    24,    25,    26,    27,    28,    29,    60,
      31,    32,    33,    34,    35,    36,     0,    37,    38,    39,
      40,    41,     9,     0,    42,     0,     0,    12,    13,     0,
      15,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,    34,
     192,    36,     0,   147,     0,    39,    40,     0,   148,   149,
     130,   150,   151,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   152,   153,   154,
     155,     0,   156,     0,     0,     0,   157,   158
};

static const yytype_int16 yycheck[] =
{
      10,    95,    91,     4,   101,   101,   113,    99,   208,    19,
      99,    17,    34,    23,   143,     6,     7,    20,   218,    25,
      30,    45,    13,    47,    15,    16,    17,    18,   117,    44,
      21,    22,    23,    42,    14,    38,    42,     6,     7,    33,
      49,    44,   136,    49,    13,    39,    15,    16,    17,    18,
       0,    42,    21,    22,    23,    33,   201,   146,    49,    39,
      45,    71,    47,    64,   160,    75,    42,   196,    42,   161,
     215,    44,   161,    42,    42,    85,   183,   174,   174,    44,
      90,   177,    48,    93,    94,   182,   182,    42,    43,    48,
      48,    39,   181,     4,     5,   192,   192,    50,    99,    44,
      35,    48,    45,   113,    45,   115,    43,    46,   118,    45,
       3,   200,    41,    44,    52,     8,     9,    43,    11,    12,
     217,   217,    33,   133,   213,    44,    51,   137,    39,    40,
      44,   141,    53,   227,    27,    28,    29,    30,    46,    32,
      42,    24,    52,    36,    37,     3,    44,    42,    41,    53,
       8,     9,    42,    11,    12,    17,    49,    48,   177,   197,
     170,   162,    49,    85,   118,   175,     5,   109,     5,    27,
      28,    29,    30,   183,    32,   217,    -1,    -1,    36,    37,
      -1,    -1,    -1,    41,    -1,   195,    -1,   197,     3,   199,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    53,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    52,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    38,    -1,
       3,    41,    42,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    34,    35,    36,    37,    38,     3,    -1,    41,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,     3,    -1,    41,    -1,    -1,     8,     9,    -1,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    30,
      31,    32,    -1,     3,    -1,    36,    37,    -1,     8,     9,
      41,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    -1,    32,    -1,    -1,    -1,    36,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    60,   105,    44,   104,     0,    33,    55,     3,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    34,    35,    36,
      37,    38,    41,    42,    57,    59,    62,    64,    65,    69,
      70,    92,    96,   102,   103,    42,     6,    10,    15,    19,
      26,    65,    20,    38,    56,    65,    58,    65,    65,    44,
      42,    48,    55,    48,    42,    43,    48,     4,     5,    39,
      40,    55,    66,    67,    68,    17,    65,    90,    91,    42,
       6,     7,    13,    15,    16,    17,    18,    21,    22,    23,
      42,    73,    74,    77,    79,    85,    92,    93,    94,    95,
      96,    65,   101,    52,    65,    86,    87,    44,    42,    49,
      67,    81,    50,    65,    70,    55,    81,    82,    83,    84,
      41,    61,    62,    63,    49,    94,    17,    25,    42,    49,
     100,    86,    65,    44,    67,    90,    35,     3,     8,     9,
      11,    12,    27,    28,    29,    30,    32,    36,    37,    78,
      48,    45,    45,    43,    65,    70,    65,    99,    53,    68,
      46,    14,    67,    80,    45,    63,    75,    76,    81,    55,
      41,    44,    52,    43,    87,    88,    89,    63,    65,    49,
      75,    67,    31,    63,    97,    86,    44,    45,    47,    51,
      44,    46,    72,    63,    53,    68,    88,    65,    67,    69,
      71,    42,    24,    44,    72,    45,    47,    52,    67,    42,
      69,    97,    72,    53,    42,    48,    98,    17,    70,    49
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    56,    56,    56,    57,    58,    58,    59,
      60,    61,    61,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    63,    63,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    65,
      65,    66,    66,    67,    68,    68,    68,    68,    69,    70,
      71,    71,    72,    72,    73,    73,    73,    73,    74,    75,
      76,    76,    77,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    84,    84,    85,    86,    86,    87,
      87,    88,    89,    89,    90,    90,    90,    91,    91,    92,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      95,    95,    96,    97,    97,    98,    98,    99,   100,   100,
     100,   101,   101,   102,   103,   103,   103,   103,   103,   103,
     103,   104,   104,   105
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     0,     4,     1,     3,     3,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       1,     3,     3,     0,     1,     1,     1,     0,     7,     6,
       1,     2,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,    11,     1,     1,     3,     1,
       1,     3,     1,     3,     1,     1,     2,     1,     2,     5,
       1,     3,     1,     3,     6,     3,     4,     1,     3,     5,
       2,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     5,     2,     1,     4,     0,     9,     2,     2,
       1,     0,     2,     5,     1,     1,     1,     1,     1,     1,
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
      yyerror (&yylloc, scanner, result, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, result); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, list* result)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (result);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, list* result)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, result);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void* scanner, list* result)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, result);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, result); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void* scanner, list* result)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (result);
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
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, void* scanner, list* result)
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
#line 109 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[0].string)); }
#line 1707 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 111 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 1; }
#line 1713 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 112 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 2; }
#line 1719 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 113 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 0; }
#line 1725 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 116 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = (yyvsp[-1].object); }
#line 1731 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 119 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[0].string)); }
#line 1737 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 120 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].string), tstr, (yyvsp[-2].object));
}
#line 1746 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 125 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons(mapconcat(to_str, (yyvsp[-1].object), "."), tstr, nil);
    del((yyvsp[-1].object));
}
#line 1756 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 132 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 1762 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 135 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[0].string)); }
#line 1768 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 136 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].string), tstr, (yyvsp[-2].object));
}
#line 1777 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 142 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "bool";       }
#line 1783 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 143 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "fixed32";    }
#line 1789 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 144 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "fixed64";    }
#line 1795 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 145 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "int32";      }
#line 1801 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 146 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "int64";      }
#line 1807 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 147 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "sfixed32";   }
#line 1813 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 148 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "sfixed64";   }
#line 1819 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 149 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "sint32";     }
#line 1825 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 150 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "sint64";     }
#line 1831 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 151 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "string";     }
#line 1837 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 152 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "uint32";     }
#line 1843 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 153 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "uint64";     }
#line 1849 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 156 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[0].string)); }
#line 1855 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 157 "protopy.y" /* yacc.c:1646  */
    { (yyval.object) = reverse((yyval.object)); }
#line 1861 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 161 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "enum";       }
#line 1867 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 162 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "extensions"; }
#line 1873 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 163 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "import";     }
#line 1879 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 164 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "message";    }
#line 1885 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 165 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "map";        }
#line 1891 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 166 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "oneof";      }
#line 1897 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 167 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "option";     }
#line 1903 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 168 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "optional";   }
#line 1909 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 169 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "package";    }
#line 1915 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 170 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "public";     }
#line 1921 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 171 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "required";   }
#line 1927 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 172 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "repeated";   }
#line 1933 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 173 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "reserved";   }
#line 1939 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 174 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "returns";    }
#line 1945 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 175 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "rpc";        }
#line 1951 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 176 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "service";    }
#line 1957 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 177 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "stream";     }
#line 1963 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 178 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "syntax";     }
#line 1969 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 179 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "to";         }
#line 1975 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 180 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "weak";       }
#line 1981 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 181 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.string) = "max";        }
#line 1987 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 186 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_ints(1, 1); }
#line 1993 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 187 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_ints(1, 0); }
#line 1999 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 190 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = (size_t)atoi((yyvsp[0].string)); }
#line 2005 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 192 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_ints(1, atoi((yyvsp[0].string))); }
#line 2011 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 193 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_ints(1, (yyvsp[0].index)); }
#line 2017 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 198 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2023 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 201 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2029 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 210 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 1; }
#line 2035 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 211 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 2; }
#line 2041 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 212 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 3; }
#line 2047 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 213 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 0; }
#line 2053 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 216 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    char* tname = mapconcat(to_str, (yyvsp[-5].object), ".");
    list pos = from_ints(1, (yyvsp[-2].index));
    (yyval.object) = tag(7, cons(tname, tstr, cons((yyvsp[-4].string), tstr, pos)));
    del((yyvsp[-5].object));
}
#line 2065 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 224 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    char* tname = mapconcat(to_str, (yyvsp[-5].object), ".");
    list pos = from_ints(1, (yyvsp[-2].index));
    (yyval.object) = tag(7, cons(tname, tstr, cons((yyvsp[-4].string), tstr, pos)));
    del((yyvsp[-5].object));
}
#line 2077 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 233 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].object), tlist, nil);
}
#line 2086 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 236 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object));
}
#line 2095 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 242 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(6, cons((yyvsp[-3].string), tstr, (yyvsp[-1].object))); }
#line 2101 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 245 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 0;  }
#line 2107 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 246 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 1;  }
#line 2113 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 247 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 2;  }
#line 2119 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 248 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 3;  }
#line 2125 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 249 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 4;  }
#line 2131 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 250 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 5;  }
#line 2137 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 251 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 6;  }
#line 2143 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 252 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 7;  }
#line 2149 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 253 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 8;  }
#line 2155 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 254 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 9;  }
#line 2161 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 255 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 10; }
#line 2167 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 256 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.index) = 11; }
#line 2173 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 260 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2179 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 263 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2185 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 264 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2191 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 267 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2197 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 268 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2203 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 271 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2209 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 274 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2215 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 275 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2221 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 281 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2227 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 284 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[0].string)); }
#line 2233 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 285 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].string), tstr, (yyvsp[-1].object));
}
#line 2242 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 291 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].object), tstr, (yyvsp[-3].object));
}
#line 2251 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 298 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2257 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 305 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = from_strings(1, (yyvsp[-5].string));
}
#line 2266 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 309 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_strings(1, (yyvsp[-2].string)); }
#line 2272 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 310 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = NULL; }
#line 2278 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 313 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_lists(1, (yyvsp[0].object)); }
#line 2284 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 314 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-2].object)); }
#line 2290 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 317 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons(from_strings(1, (yyvsp[-3].string)), tstr, (yyvsp[-1].object));
}
#line 2299 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 323 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2305 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 329 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2311 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 330 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2317 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 331 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = (yyvsp[0].object); }
#line 2323 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 332 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2329 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 333 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2335 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 334 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2341 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 337 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = from_lists(1, (yyvsp[0].object));
}
#line 2350 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 341 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object));
}
#line 2359 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 347 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[-3].string), tstr, (yyvsp[-1].object));
}
#line 2368 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 353 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = (yyvsp[0].object); }
#line 2374 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 357 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2380 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 358 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.nothing) = NULL; }
#line 2386 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 361 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = from_strings(1, (yyvsp[-8].string));
}
#line 2395 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 367 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2401 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 368 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = (yyvsp[0].object); }
#line 2407 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 369 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2413 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 373 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = from_lists(1, nil); }
#line 2419 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 374 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT; 
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object));
}
#line 2428 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 380 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[-3].string), tstr, (yyvsp[-1].object));
}
#line 2437 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 386 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(0, (yyvsp[0].object)); }
#line 2443 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 387 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(1, (yyvsp[0].object)); }
#line 2449 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 388 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(3, (yyvsp[0].object)); }
#line 2455 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 389 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(4, (yyvsp[0].object)); }
#line 2461 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 390 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = tag(5, (yyvsp[0].object)); }
#line 2467 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 391 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2473 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 392 "protopy.y" /* yacc.c:1646  */
    { MAYBE_ABORT; (yyval.object) = nil; }
#line 2479 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 395 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = from_lists(1, nil);
}
#line 2488 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 399 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = cons((yyvsp[0].object), tlist, (yyvsp[-1].object));
}
#line 2497 "protopy.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 405 "protopy.y" /* yacc.c:1646  */
    {
    MAYBE_ABORT;
    (yyval.object) = (yyvsp[0].object);
    *result = (yyval.object);
}
#line 2507 "protopy.tab.c" /* yacc.c:1646  */
    break;


#line 2511 "protopy.tab.c" /* yacc.c:1646  */
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
      yyerror (&yylloc, scanner, result, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, result, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, result);
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
                  yystos[yystate], yyvsp, yylsp, scanner, result);
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
  yyerror (&yylloc, scanner, result, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, result);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, result);
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
#line 411 "protopy.y" /* yacc.c:1906  */

