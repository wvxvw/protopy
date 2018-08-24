/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */
#line 46 "protopy.y" /* yacc.c:339  */

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


#line 82 "protopy.tab.c" /* yacc.c:339  */

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
/* "%code requires" blocks.  */
#line 9 "protopy.y" /* yacc.c:355  */

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

#line 149 "protopy.tab.c" /* yacc.c:355  */

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

/* Value type.  */

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

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, void* scanner, proto_file_t* pf);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);

#endif /* !YY_YY_PROTOPY_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 233 "protopy.tab.c" /* yacc.c:358  */

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
#define YYLAST   798

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  169
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  268

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
       0,   131,   131,   133,   134,   135,   138,   144,   148,   153,
     159,   162,   166,   171,   176,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   193,   197,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   223,   223,   226,   227,   230,   234,   235,   236,   237,
     238,   240,   241,   243,   244,   246,   247,   248,   254,   257,
     260,   260,   263,   263,   266,   267,   268,   269,   272,   283,
     292,   293,   296,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   313,   323,   324,   327,   328,
     331,   331,   334,   335,   338,   338,   341,   344,   344,   346,
     347,   349,   350,   353,   354,   357,   360,   361,   363,   364,
     367,   372,   373,   376,   377,   380,   380,   392,   392,   400,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     415,   416,   419,   422,   423,   425,   425,   441,   442,   445,
     446,   447,   450,   455,   456,   457,   461,   462,   465,   468,
     469,   470,   471,   472,   473,   474,   475,   478,   479,   482
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
  "enum_fields", "enum", "$@1", "extend", "$@2", "extensions",
  "message_field", "message_block", "message_header", "message_tail",
  "message", "$@3", "rpc_type", "rpc_options", "rpc", "service_body_part",
  "service_body", "service", "top_level", "top_levels", "s", YY_NULLPTR
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

#define YYPACT_NINF -210

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-210)))

#define YYTABLE_NINF -53

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -18,   -25,  -210,    53,    23,   113,  -210,  -210,    17,   669,
      31,     2,   669,   381,   669,   669,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,   -17,    36,  -210,  -210,  -210,    50,
      38,  -210,   706,  -210,    45,  -210,  -210,    46,    47,    22,
    -210,    48,    21,    52,    36,    51,    57,    62,  -210,   -22,
    -210,   706,    14,  -210,   669,  -210,    54,    55,   669,   381,
    -210,  -210,  -210,    -4,  -210,  -210,   591,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,   511,  -210,  -210,   631,  -210,
     743,  -210,    56,  -210,  -210,  -210,  -210,  -210,   243,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,    24,    72,  -210,   760,
      63,  -210,  -210,  -210,    64,    67,  -210,  -210,  -210,   669,
    -210,  -210,   381,  -210,    73,  -210,   473,   533,   706,  -210,
      71,  -210,    77,    80,  -210,   289,    81,   381,   669,  -210,
    -210,  -210,     9,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,    83,   591,    54,    23,    89,
      91,    54,  -210,  -210,  -210,    46,   176,  -210,  -210,   427,
    -210,    84,  -210,  -210,  -210,  -210,   591,   669,  -210,   164,
    -210,  -210,    54,   176,    87,  -210,   335,   555,    90,    99,
    -210,  -210,    96,  -210,   381,  -210,  -210,   591,  -210,    92,
     669,    54,   381,   105,   106,  -210,    25,  -210,   125,   108,
      96,  -210,    28,  -210,   176,   381,  -210,   101,    54,   116,
     381,  -210,  -210,  -210,   555,    96,  -210,  -210,   109,   117,
     118,  -210,     3,  -210,   381,  -210,   115,  -210
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   167,     0,     0,   169,     1,     2,     0,     0,
       0,     5,     0,     0,     0,     0,   166,   162,   163,   160,
     165,   145,   159,   161,   168,    10,    15,    30,    31,    16,
      17,    32,    18,    19,    34,    50,    33,    35,    36,    37,
      38,    39,    41,    40,    42,    43,    44,    45,    20,    21,
      22,    23,    46,    24,    47,    48,    25,    26,    49,    52,
      29,    51,   125,    11,     0,    14,   127,     4,     3,     0,
       0,   107,     0,   108,     0,   164,   109,   114,     0,     0,
       7,     0,    77,     0,    13,     0,     0,     0,   142,     0,
      69,     0,     0,     9,     0,   156,     0,     0,     0,     0,
      76,    75,    74,     0,   139,   144,     0,   135,   132,   134,
     136,   130,   138,   137,   140,    77,   146,   131,     0,    12,
      77,     6,   111,   110,    54,    53,    55,    56,     0,    58,
      60,    59,    57,    65,    68,     8,     0,    99,   129,     0,
       0,   133,   102,   100,   104,   105,   106,    28,    27,     0,
     143,   141,    36,   122,     0,   123,     0,    77,     0,   113,
     107,    67,   108,     0,    63,     0,     0,     0,     0,   155,
     158,   157,     0,    93,    89,    90,    83,    84,    91,    92,
      87,    88,    94,    85,    86,     0,     0,     0,     0,     0,
       0,     0,   126,   124,   128,   112,     0,    66,    64,     0,
     153,     0,   154,    96,    97,    98,     0,     0,    80,     0,
     101,   103,     0,     0,   118,    61,     0,     0,     0,     0,
      82,    81,    73,   121,     0,   120,    62,     0,   148,     0,
       0,     0,     0,     0,     0,   116,     0,   147,     0,     0,
      73,    70,     0,    78,     0,     0,   119,     0,     0,     0,
       0,    72,   115,   117,     0,    73,    79,    71,     0,     0,
     151,    95,     0,   152,     0,   150,     0,   149
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -210,     0,  -210,  -210,  -210,  -210,  -210,   102,    -9,   -97,
     -92,   -13,    -7,  -210,   -90,  -181,  -154,   -30,  -210,  -180,
     -96,  -210,  -209,  -210,  -210,   -39,  -210,  -210,  -210,  -210,
    -210,   -91,  -210,  -210,  -210,  -210,    88,   -56,  -210,  -118,
     -74,  -210,  -210,    16,  -210,   170,  -210,   173,  -210,  -210,
     -89,    65,  -210,  -210,   177,  -210,   -71,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   129,    69,    17,    79,    18,     2,    65,   147,    60,
     207,    61,   163,   131,   132,   133,   164,   165,   134,    74,
      75,   242,   233,   106,   107,   208,   209,   108,   185,   109,
     205,   138,   144,   145,   146,   110,    76,    77,   159,    78,
     235,   236,   225,   155,   156,   111,    83,   112,    86,   113,
     114,   115,    21,   116,   117,    82,   229,   263,   202,   171,
     136,    23,    24,     5,     3
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      73,    66,    62,   141,     8,    70,   137,    80,    81,   148,
     166,   198,   143,   137,   149,   215,    89,     1,   124,   125,
       4,   264,    91,    67,   203,    63,   151,     9,    96,    10,
       7,   249,   223,   122,   190,    97,   126,    12,    98,    99,
     100,    68,   167,   101,   102,   103,   259,   166,     7,   126,
     168,   265,   241,     6,   126,   127,    63,     7,    64,    73,
      25,   128,   198,   252,   104,    93,    94,   169,   151,   105,
     257,   200,   170,    63,   245,    64,   246,   250,    73,   251,
      85,   166,   204,   130,    87,    88,    73,   135,    90,   148,
      91,   140,    92,   119,   126,    95,   210,   137,   166,   118,
     158,   214,   195,   142,   120,   121,   234,   139,   172,   148,
     186,   154,   148,   187,   218,   162,   188,   -52,   191,     9,
     148,    10,   222,   -51,    11,   228,   196,   234,   199,    12,
     148,    13,   206,    14,   212,   237,   213,   224,   217,    73,
      15,   240,   189,   230,   231,    73,   232,   238,   243,   154,
     247,   244,   162,   248,    73,   254,    16,   148,   255,   256,
     261,   201,   228,   267,   260,   262,    84,    26,   266,   216,
     221,   253,   193,    29,    30,    19,    32,    33,    20,   123,
     124,   125,    22,   258,     0,   157,   162,   130,   211,     0,
       0,     0,    48,    49,    50,    51,     0,    53,     0,     0,
     219,    56,    57,   162,   130,     0,    63,     0,    64,     0,
       7,    73,   220,     0,     0,     0,   126,   127,    63,    73,
      64,     0,     0,   239,     0,     0,     0,     0,     0,     0,
       0,     0,    73,     0,     0,   130,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,    27,
      28,    73,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,     0,    54,    55,
      56,    57,    58,     0,     0,   160,     0,     0,     0,     0,
       0,   161,    26,     0,     0,    27,    28,    72,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,     0,    54,    55,    56,    57,    58,     0,
       0,   160,     0,     0,     0,     0,     0,   197,    26,     0,
       0,    27,    28,    72,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,     0,
      54,    55,    56,    57,    58,     0,     0,   160,     0,     0,
       0,     0,     0,   226,    26,     0,     0,    27,    28,    72,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,     0,    54,    55,    56,    57,
      58,     0,     0,    71,     0,     0,     0,     0,     0,     0,
      26,     0,     0,    27,    28,    72,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,     0,    54,    55,    56,    57,    58,     0,     0,   160,
       0,     0,     0,     0,     0,     0,    26,     0,     0,    27,
      28,    72,    29,    30,    31,    32,    33,    34,    35,    36,
      37,   152,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,     0,    54,    55,
      56,    57,    58,     0,     0,    59,   153,     9,    96,    10,
       0,   192,     0,     0,     0,    97,     0,    12,    98,    99,
     100,     0,     0,   101,   102,   103,     0,     0,     0,     9,
      96,    10,     0,     0,     0,     0,     0,    97,     0,    12,
      98,    99,   100,     0,   104,   101,   102,   103,    26,   150,
       0,     0,     0,     0,    29,    30,     0,    32,    33,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,   194,     0,    48,    49,    50,    51,   227,    53,     0,
       0,     0,    56,    57,    26,     0,     0,    63,     0,    64,
      29,    30,     0,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,    51,     0,    53,     0,     0,     0,    56,    57,
       0,     0,     0,    63,    26,    64,     0,    27,    28,     0,
      29,    30,    31,    32,    33,    34,    35,    36,    37,   152,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,     0,    54,    55,    56,    57,
      58,     0,    26,    59,   153,    27,    28,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,     0,    54,    55,    56,    57,    58,    26,
       0,    59,    27,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       0,    54,    55,    56,    57,    58,     0,     0,    71,     9,
      96,    10,     0,     0,     0,     0,     0,    97,     0,    12,
      98,    99,   100,   173,     0,   101,   102,   103,     0,   174,
     175,     0,   176,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,   178,   179,
     180,   181,     0,   182,     0,     0,     0,   183,   184
};

static const yytype_int16 yycheck[] =
{
      13,    10,     9,    99,     4,    12,    96,    14,    15,   106,
     128,   165,   103,   103,   106,   196,    72,    35,     4,     5,
      45,    18,    44,    21,    15,    42,   115,     6,     7,     8,
      34,   240,   213,    55,   152,    14,    40,    16,    17,    18,
      19,    39,    18,    22,    23,    24,   255,   165,    34,    40,
      26,    48,   232,     0,    40,    41,    42,    34,    44,    72,
      43,    47,   216,   244,    43,    43,    44,    43,   157,    48,
     250,   167,    48,    42,    49,    44,    51,    49,    91,    51,
      44,   199,   172,    92,    34,    47,    99,    94,    43,   186,
      44,    98,    45,    42,    40,    47,   187,   187,   216,    47,
      44,   191,   158,   103,    47,    43,   224,    52,    36,   206,
      47,   118,   209,    49,   206,   128,    49,    46,    45,     6,
     217,     8,   212,    46,    11,   217,    46,   245,    47,    16,
     227,    18,    49,    20,    45,   227,    45,    50,    54,   152,
      27,   231,   149,    53,    45,   158,    50,    55,    43,   156,
      25,    45,   165,    45,   167,    54,    43,   254,   248,    43,
      43,   168,   254,    48,    55,    47,    64,     3,   264,   199,
     209,   245,   156,     9,    10,     5,    12,    13,     5,    91,
       4,     5,     5,   254,    -1,   120,   199,   196,   188,    -1,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    -1,    -1,
     207,    37,    38,   216,   213,    -1,    42,    -1,    44,    -1,
      34,   224,    48,    -1,    -1,    -1,    40,    41,    42,   232,
      44,    -1,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   245,    -1,    -1,   244,    -1,   250,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,     6,
       7,   264,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,     3,    -1,    -1,     6,     7,    54,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,     3,    -1,
      -1,     6,     7,    54,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,     3,    -1,    -1,     6,     7,    54,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,     6,     7,    54,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,     6,
       7,    54,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    -1,    -1,    42,    43,     6,     7,     8,
      -1,    48,    -1,    -1,    -1,    14,    -1,    16,    17,    18,
      19,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,     6,
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
      17,    18,    19,     3,    -1,    22,    23,    24,    -1,     9,
      10,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    28,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    37,    38
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    35,    62,   120,    45,   119,     0,    34,    57,     6,
       8,    11,    16,    18,    20,    27,    43,    59,    61,   101,
     103,   108,   110,   117,   118,    43,     3,     6,     7,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    35,    36,    37,    38,    39,    42,
      65,    67,    68,    42,    44,    63,    64,    21,    39,    58,
      68,    42,    54,    67,    75,    76,    92,    93,    95,    60,
      68,    68,   111,   102,    63,    44,   104,    34,    47,    93,
      43,    44,    45,    43,    44,    47,     7,    14,    17,    18,
      19,    22,    23,    24,    43,    48,    79,    80,    83,    85,
      91,   101,   103,   105,   106,   107,   109,   110,    47,    42,
      47,    43,    55,    92,     4,     5,    40,    41,    47,    57,
      64,    69,    70,    71,    74,    68,   116,    70,    87,    52,
      68,    76,    57,    87,    88,    89,    90,    64,    65,    66,
      48,   106,    18,    43,    68,    99,   100,   107,    44,    94,
      42,    48,    67,    68,    72,    73,    95,    18,    26,    43,
      48,   115,    36,     3,     9,    10,    12,    13,    28,    29,
      30,    31,    33,    37,    38,    84,    47,    49,    49,    68,
      95,    45,    48,    99,    48,    93,    46,    48,    72,    47,
      76,    68,   114,    15,    70,    86,    49,    66,    81,    82,
      87,    57,    45,    45,    70,    71,    73,    54,    66,    68,
      48,    81,    70,    71,    50,    98,    48,    32,    66,   112,
      53,    45,    50,    78,    95,    96,    97,    66,    55,    68,
      70,    75,    77,    43,    45,    49,    51,    25,    45,    78,
      49,    51,    71,    96,    54,    70,    43,    75,   112,    78,
      55,    43,    47,   113,    18,    48,    76,    48
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
      71,    72,    72,    73,    73,    74,    74,    74,    75,    76,
      77,    77,    78,    78,    79,    79,    79,    79,    80,    81,
      82,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    85,    86,    86,    87,    87,
      88,    88,    89,    89,    90,    90,    91,    92,    92,    93,
      93,    94,    94,    95,    95,    96,    97,    97,    98,    98,
      99,    99,    99,   100,   100,   102,   101,   104,   103,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   107,   108,   109,   109,   111,   110,   112,   112,   113,
     113,   113,   114,   115,   115,   115,   116,   116,   117,   118,
     118,   118,   118,   118,   118,   118,   118,   119,   119,   120
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
       1,     3,     4,     1,     2,     1,     3,     2,     3,     2,
       1,     3,     3,     0,     1,     1,     1,     0,     7,     6,
       1,     2,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,    11,     1,     1,     3,     1,
       1,     3,     1,     3,     1,     1,     2,     1,     1,     1,
       3,     0,     2,     4,     1,     3,     1,     3,     0,     3,
       4,     4,     1,     1,     2,     0,     6,     0,     6,     2,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     2,     1,     0,     3,     2,     1,     4,
       2,     0,     9,     2,     2,     1,     0,     2,     5,     1,
       1,     1,     1,     1,     2,     1,     1,     0,     2,     2
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
      yyerror (&yylloc, scanner, pf, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, pf); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, proto_file_t* pf)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (pf);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, proto_file_t* pf)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, pf);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void* scanner, proto_file_t* pf)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, pf);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, pf); \
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
    default: /* Avoid compiler warnings. */
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void* scanner, proto_file_t* pf)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (pf);
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
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, void* scanner, proto_file_t* pf)
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 131 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 1769 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 3:
#line 133 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 1; }
#line 1775 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 4:
#line 134 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 2; }
#line 1781 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 5:
#line 135 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 0; }
#line 1787 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 6:
#line 138 "protopy.y" /* yacc.c:1648  */
    {
    // TODO(olegs): Imorts need to be a prefix tree.
    APR_ARRAY_PUSH(pf->imports, apr_array_header_t*) = parse_import((yyvsp[-1].string), pf->mp);
}
#line 1796 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 7:
#line 144 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.array) = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH((yyval.array), char*) = (yyvsp[0].string);
}
#line 1805 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 8:
#line 148 "protopy.y" /* yacc.c:1648  */
    {
    APR_ARRAY_PUSH((yyvsp[-2].array), char*) = (yyvsp[0].string);
    (yyval.array) = (yyvsp[-2].array);
}
#line 1814 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 9:
#line 153 "protopy.y" /* yacc.c:1648  */
    {
    pf->package = apr_array_pstrcat(pf->mp, (yyvsp[-1].array), '.');
    (yyval.nothing) = NULL;
}
#line 1823 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 10:
#line 159 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 1829 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 11:
#line 162 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.array) = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH((yyval.array), char*) = (yyvsp[0].string);
}
#line 1838 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 12:
#line 166 "protopy.y" /* yacc.c:1648  */
    {
    APR_ARRAY_PUSH((yyval.array), char*) = (yyvsp[0].string);
    (yyval.array) = (yyvsp[-2].array);
}
#line 1847 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 13:
#line 171 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.array) = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH((yyval.array), char*) = ".";
    apr_array_cat((yyval.array), (yyvsp[0].array));
}
#line 1857 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 15:
#line 179 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "bool");       }
#line 1863 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 16:
#line 180 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "fixed32");    }
#line 1869 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 17:
#line 181 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "fixed64");    }
#line 1875 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 18:
#line 182 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "int32");      }
#line 1881 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 19:
#line 183 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "int64");      }
#line 1887 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 20:
#line 184 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "sfixed32");   }
#line 1893 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 21:
#line 185 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "sfixed64");   }
#line 1899 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 22:
#line 186 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "sint32");     }
#line 1905 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 23:
#line 187 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "sint64");     }
#line 1911 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 24:
#line 188 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "string");     }
#line 1917 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 25:
#line 189 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "uint32");     }
#line 1923 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 26:
#line 190 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "uint64");     }
#line 1929 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 27:
#line 193 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.array) = apr_array_make(pf->mp, 1, sizeof(char*));
    APR_ARRAY_PUSH((yyval.array), char*) = (yyvsp[0].string);
}
#line 1938 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 30:
#line 201 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "enum");       }
#line 1944 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 31:
#line 202 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "extensions"); }
#line 1950 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 32:
#line 203 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "import");     }
#line 1956 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 33:
#line 204 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "message");    }
#line 1962 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 34:
#line 205 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "map");        }
#line 1968 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 35:
#line 206 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "oneof");      }
#line 1974 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 36:
#line 207 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "option");     }
#line 1980 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 37:
#line 208 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "optional");   }
#line 1986 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 38:
#line 209 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "package");    }
#line 1992 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 39:
#line 210 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "public");     }
#line 1998 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 40:
#line 211 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "required");   }
#line 2004 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 41:
#line 212 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "repeated");   }
#line 2010 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 42:
#line 213 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "reserved");   }
#line 2016 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 43:
#line 214 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "returns");    }
#line 2022 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 44:
#line 215 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "rpc");        }
#line 2028 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 45:
#line 216 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "service");    }
#line 2034 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 46:
#line 217 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "stream");     }
#line 2040 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 47:
#line 218 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "syntax");     }
#line 2046 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 48:
#line 219 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "to");         }
#line 2052 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 49:
#line 220 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "weak");       }
#line 2058 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 50:
#line 221 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = apr_pstrdup(pf->mp, "max");        }
#line 2064 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 53:
#line 226 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 1; }
#line 2070 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 54:
#line 227 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 0; }
#line 2076 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 55:
#line 230 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.index) = (int)apr_atoi64((yyvsp[0].string));
}
#line 2084 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 56:
#line 234 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2090 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 57:
#line 235 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2096 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 58:
#line 236 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2102 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 59:
#line 237 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2108 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 60:
#line 238 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2114 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 61:
#line 240 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2120 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 62:
#line 241 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2126 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 65:
#line 246 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2132 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 66:
#line 247 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2138 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 67:
#line 248 "protopy.y" /* yacc.c:1648  */
    {
    /* protoc has a bug where it allows option declaration without value */
    (yyval.nothing) = NULL;
 }
#line 2147 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 68:
#line 254 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2153 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 69:
#line 257 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2159 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 74:
#line 266 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 1; }
#line 2165 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 75:
#line 267 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 2; }
#line 2171 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 76:
#line 268 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 3; }
#line 2177 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 77:
#line 269 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = 0; }
#line 2183 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 78:
#line 272 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        proto_field_t* field = make_proto_field((yyvsp[-4].string), (yyvsp[-5].array), (yyvsp[-2].index), pf);
        apr_array_header_t* arr = ((yyvsp[-6].index) == 2) ?
            pf->current_message->repeated :
            pf->current_message->fields;
        APR_ARRAY_PUSH(arr, proto_field_t*) = field;
    }
    (yyval.nothing) = NULL;
}
#line 2198 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 79:
#line 283 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        APR_ARRAY_PUSH(pf->current_message->fields, proto_field_t*) =
            make_proto_field((yyvsp[-4].string), (yyvsp[-5].array), (yyvsp[-2].index), pf);
    }
    (yyval.nothing) = NULL;
}
#line 2210 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 82:
#line 296 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2216 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 83:
#line 299 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_int32;    }
#line 2222 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 84:
#line 300 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_int64;    }
#line 2228 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 85:
#line 301 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_uint32;   }
#line 2234 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 86:
#line 302 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_uint64;   }
#line 2240 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 87:
#line 303 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_sint32;   }
#line 2246 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 88:
#line 304 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_sint64;   }
#line 2252 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 89:
#line 305 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_fixed32;  }
#line 2258 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 90:
#line 306 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_fixed64;  }
#line 2264 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 91:
#line 307 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_sfixed32; }
#line 2270 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 92:
#line 308 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_sfixed64; }
#line 2276 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 93:
#line 309 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_bool;     }
#line 2282 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 94:
#line 310 "protopy.y" /* yacc.c:1648  */
    { (yyval.index) = vt_string;   }
#line 2288 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 95:
#line 314 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        APR_ARRAY_PUSH(pf->current_message->maps, proto_map_field_t*) =
            make_proto_map_field((yyvsp[-4].string), (yyvsp[-8].index), (yyvsp[-6].array), (yyvsp[-2].index), pf);
    }
    (yyval.nothing) = NULL;
}
#line 2300 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 96:
#line 323 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2306 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 97:
#line 324 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2312 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 98:
#line 327 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2318 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 99:
#line 328 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2324 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 101:
#line 331 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2330 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 102:
#line 334 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2336 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 103:
#line 335 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2342 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 106:
#line 341 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2348 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 109:
#line 346 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2354 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 110:
#line 347 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2360 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 111:
#line 349 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2366 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 112:
#line 350 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2372 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 113:
#line 353 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2378 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 115:
#line 357 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2384 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 118:
#line 363 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2390 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 119:
#line 364 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2396 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 120:
#line 367 "protopy.y" /* yacc.c:1648  */
    {
    APR_ARRAY_PUSH(pf->current_enum->members, proto_enum_member_t*) =
        make_proto_enum_member((yyvsp[-3].string), (yyvsp[-1].index), pf->mp);
    (yyval.nothing) = NULL;
}
#line 2406 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 121:
#line 372 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2412 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 122:
#line 373 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2418 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 123:
#line 376 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2424 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 124:
#line 377 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2430 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 125:
#line 380 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        pf->current_enum = make_proto_enum((yyvsp[0].string), pf);
    }
}
#line 2440 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 126:
#line 384 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        APR_ARRAY_PUSH(pf->enums, proto_enum_t*) = pf->current_enum;
    }
    (yyval.nothing) = NULL;
}
#line 2451 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 127:
#line 392 "protopy.y" /* yacc.c:1648  */
    {
    pf->need = false;
}
#line 2459 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 128:
#line 394 "protopy.y" /* yacc.c:1648  */
    {
    pf->need = true;
    (yyval.nothing) = NULL;
}
#line 2468 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 129:
#line 400 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2474 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 133:
#line 406 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2480 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 139:
#line 412 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2486 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 140:
#line 415 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2492 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 141:
#line 416 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2498 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 142:
#line 419 "protopy.y" /* yacc.c:1648  */
    { (yyval.string) = (yyvsp[-1].string); }
#line 2504 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 143:
#line 422 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2510 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 144:
#line 423 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2516 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 145:
#line 425 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        APR_ARRAY_PUSH(pf->scope, char*) = (yyvsp[0].string);
        APR_ARRAY_PUSH(pf->previous, proto_message_t*) = pf->current_message;
        pf->current_message = make_proto_message(pf->scope, pf);
    }
}
#line 2528 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 146:
#line 431 "protopy.y" /* yacc.c:1648  */
    {
    if (pf->need) {
        APR_ARRAY_PUSH(pf->messages, proto_message_t*) = pf->current_message;
        proto_message_t** m = apr_array_pop(pf->previous);
        pf->current_message = *m;
        apr_array_pop(pf->scope);
    }
}
#line 2541 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 147:
#line 441 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2547 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 148:
#line 442 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2553 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 149:
#line 445 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2559 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 150:
#line 446 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2565 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 151:
#line 447 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2571 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 152:
#line 450 "protopy.y" /* yacc.c:1648  */
    {
    (yyval.nothing) = NULL;
}
#line 2579 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 153:
#line 455 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2585 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 154:
#line 456 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2591 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 155:
#line 457 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2597 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 156:
#line 461 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2603 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 157:
#line 462 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2609 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 158:
#line 465 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2615 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 164:
#line 473 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2621 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 166:
#line 475 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2627 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 167:
#line 478 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2633 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 168:
#line 479 "protopy.y" /* yacc.c:1648  */
    { (yyval.nothing) = NULL; }
#line 2639 "protopy.tab.c" /* yacc.c:1648  */
    break;

  case 169:
#line 482 "protopy.y" /* yacc.c:1648  */
    {
    qualify_types(pf);
    (yyval.nothing) = NULL;
}
#line 2648 "protopy.tab.c" /* yacc.c:1648  */
    break;


#line 2652 "protopy.tab.c" /* yacc.c:1648  */
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
      yyerror (&yylloc, scanner, pf, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, pf, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, pf);
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
                  yystos[yystate], yyvsp, yylsp, scanner, pf);
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
  yyerror (&yylloc, scanner, pf, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, pf);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, pf);
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
#line 487 "protopy.y" /* yacc.c:1907  */

