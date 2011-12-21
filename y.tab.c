
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "rules.y"

#include <stdio.h>
#include <string.h>

#include "target.h"
#include "stringbuilder.h"
#include "src_gatherer.h"

#define YYSTYPE char*

extern char* yytext;

extern int yylex (void);
extern int yyget_lineno();
extern void set_parsererror(int, int, char*);
extern void check_namgen_version(const char *);

static target_entry *current_target = NULL;
static stringbuilder *sb = NULL;

void parser_setup(void)
{
   current_target = NULL;
   sb = sb_new();
}

void parser_cleanup(void)
{
   current_target = NULL;
   sb_destroy(sb, 1);
}

void yyerror(const char *str)
{
    set_parsererror(1, yyget_lineno(), yytext);    
}
 
int yywrap()
{
   return 1;
}
  


/* Line 189 of yacc.c  */
#line 118 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM = 258,
     LIBRARY = 259,
     WORKER = 260,
     COMMAND = 261,
     SRC = 262,
     DEPENDS = 263,
     FLAGS = 264,
     LIBS = 265,
     LDFLAGS = 266,
     ADD_OBJECTS = 267,
     DESTDIR = 268,
     SKIP_INSTALL = 269,
     SKIP_SHARED = 270,
     SKIP_STATIC = 271,
     TRUE_VALUE = 272,
     FALSE_VALUE = 273,
     VERSION_INFO = 274,
     VERSION_NUMBER = 275,
     THREE_NUMBERS = 276,
     VARIABLE = 277,
     WORD = 278,
     WILDCARD = 279,
     FILENAME = 280,
     STUFF = 281,
     EXPR_MARK = 282,
     EQUALS = 283,
     QUOTE = 284,
     OBRACE = 285,
     EBRACE = 286,
     EXPORT_INC = 287,
     CONVENIENCE_LIB = 288,
     OCOMMENT = 289,
     ECOMMENT = 290,
     SKIP_IF = 291,
     CLEAN_FILES = 292,
     SRC_EXTENSION = 293,
     REQUIRE_NAMGEN_VERSION = 294,
     CONF_MAKE_INSTALL = 295,
     SRC_DIR = 296
   };
#endif
/* Tokens.  */
#define PROGRAM 258
#define LIBRARY 259
#define WORKER 260
#define COMMAND 261
#define SRC 262
#define DEPENDS 263
#define FLAGS 264
#define LIBS 265
#define LDFLAGS 266
#define ADD_OBJECTS 267
#define DESTDIR 268
#define SKIP_INSTALL 269
#define SKIP_SHARED 270
#define SKIP_STATIC 271
#define TRUE_VALUE 272
#define FALSE_VALUE 273
#define VERSION_INFO 274
#define VERSION_NUMBER 275
#define THREE_NUMBERS 276
#define VARIABLE 277
#define WORD 278
#define WILDCARD 279
#define FILENAME 280
#define STUFF 281
#define EXPR_MARK 282
#define EQUALS 283
#define QUOTE 284
#define OBRACE 285
#define EBRACE 286
#define EXPORT_INC 287
#define CONVENIENCE_LIB 288
#define OCOMMENT 289
#define ECOMMENT 290
#define SKIP_IF 291
#define CLEAN_FILES 292
#define SRC_EXTENSION 293
#define REQUIRE_NAMGEN_VERSION 294
#define CONF_MAKE_INSTALL 295
#define SRC_DIR 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 242 "y.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   134

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNRULES -- Number of states.  */
#define YYNSTATES  160

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    22,    27,    28,    33,    34,    39,    40,    45,
      48,    51,    55,    56,    59,    61,    63,    65,    67,    69,
      71,    73,    75,    77,    79,    81,    83,    85,    87,    89,
      91,    93,    95,    97,    99,   103,   107,   108,   111,   113,
     115,   117,   119,   121,   123,   125,   127,   129,   131,   133,
     135,   137,   139,   141,   143,   145,   147,   148,   153,   157,
     161,   162,   167,   168,   173,   177,   181,   185,   189,   191,
     193,   195,   197,   198,   203,   204,   209,   210,   215,   216,
     221,   225,   229,   233,   237,   239,   241,   243,   246,   248,
     251,   253,   255,   257,   259,   261,   263,   265,   268,   270,
     272,   274
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    -1,    43,    44,    -1,    45,    -1,    47,
      -1,    49,    -1,    51,    -1,    55,    -1,    53,    -1,    54,
      -1,    -1,     3,    58,    46,    59,    -1,    -1,     4,    58,
      48,    59,    -1,    -1,     5,    58,    50,    59,    -1,    -1,
      40,    58,    52,    59,    -1,    36,    23,    -1,    39,    23,
      -1,    34,    56,    35,    -1,    -1,    56,    57,    -1,     7,
      -1,    41,    -1,    38,    -1,     8,    -1,     9,    -1,    10,
      -1,    11,    -1,    12,    -1,    32,    -1,    13,    -1,    14,
      -1,    15,    -1,    16,    -1,    33,    -1,    19,    -1,    20,
      -1,    17,    -1,    18,    -1,    28,    -1,    93,    -1,    29,
      23,    29,    -1,    30,    60,    31,    -1,    -1,    60,    61,
      -1,    62,    -1,    65,    -1,    64,    -1,    71,    -1,    72,
      -1,    76,    -1,    78,    -1,    80,    -1,    73,    -1,    66,
      -1,    68,    -1,    82,    -1,    70,    -1,    84,    -1,    85,
      -1,    86,    -1,    87,    -1,    55,    -1,    -1,     7,    28,
      63,    90,    -1,    38,    28,    23,    -1,    41,    28,    93,
      -1,    -1,    12,    28,    67,    90,    -1,    -1,    37,    28,
      69,    90,    -1,     6,    28,    93,    -1,     8,    28,    89,
      -1,    13,    28,    25,    -1,    75,    28,    74,    -1,    21,
      -1,    22,    -1,    20,    -1,    19,    -1,    -1,     9,    28,
      77,    92,    -1,    -1,    11,    28,    79,    92,    -1,    -1,
      10,    28,    81,    92,    -1,    -1,    32,    28,    83,    92,
      -1,    14,    28,    88,    -1,    15,    28,    88,    -1,    16,
      28,    88,    -1,    33,    28,    88,    -1,    17,    -1,    18,
      -1,    23,    -1,    89,    23,    -1,    91,    -1,    90,    91,
      -1,    23,    -1,    25,    -1,    26,    -1,    24,    -1,    22,
      -1,    27,    -1,    93,    -1,    92,    93,    -1,    23,    -1,
      22,    -1,    25,    -1,    26,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    54,    54,    55,    59,    59,    59,    59,    59,    59,
      59,    64,    63,    75,    74,    86,    85,    97,    96,   106,
     108,   111,   114,   115,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   142,   148,   151,   152,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   176,   176,   180,   184,
     188,   188,   192,   192,   196,   200,   204,   210,   213,   213,
     216,   217,   220,   220,   224,   224,   228,   228,   232,   232,
     236,   240,   244,   248,   252,   253,   256,   258,   262,   263,
     267,   268,   269,   270,   271,   272,   275,   277,   281,   282,
     283,   284
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "LIBRARY", "WORKER",
  "COMMAND", "SRC", "DEPENDS", "FLAGS", "LIBS", "LDFLAGS", "ADD_OBJECTS",
  "DESTDIR", "SKIP_INSTALL", "SKIP_SHARED", "SKIP_STATIC", "TRUE_VALUE",
  "FALSE_VALUE", "VERSION_INFO", "VERSION_NUMBER", "THREE_NUMBERS",
  "VARIABLE", "WORD", "WILDCARD", "FILENAME", "STUFF", "EXPR_MARK",
  "EQUALS", "QUOTE", "OBRACE", "EBRACE", "EXPORT_INC", "CONVENIENCE_LIB",
  "OCOMMENT", "ECOMMENT", "SKIP_IF", "CLEAN_FILES", "SRC_EXTENSION",
  "REQUIRE_NAMGEN_VERSION", "CONF_MAKE_INSTALL", "SRC_DIR", "$accept",
  "declarations", "declaration", "program_def", "$@1", "library_def",
  "$@2", "worker_def", "$@3", "cmi_def", "$@4", "skip_condition",
  "require_version", "comment_line", "comment_data", "comment_blub",
  "quoted_name", "def_content", "def_statements", "def_statement",
  "src_statement", "$@5", "src_ext_statement", "src_dir_statement",
  "add_objects_statement", "$@6", "clean_files_statement", "$@7",
  "command_statement", "depends_statement", "destdir_statement",
  "version_num_statement", "three_numbers_or_variable", "vnum_schema",
  "flags_statement", "$@8", "ld_flags_statement", "$@9", "libs_statement",
  "$@10", "export_inc_statement", "$@11", "skip_install_statement",
  "skip_shared_statement", "skip_static_statement",
  "convenience_statement", "true_false", "list_of_depends",
  "list_of_src_expr", "src_expr", "list_of_stuff",
  "word_variable_filename_stuff", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      44,    46,    45,    48,    47,    50,    49,    52,    51,    53,
      54,    55,    56,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    58,    59,    60,    60,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    63,    62,    64,    65,
      67,    66,    69,    68,    70,    71,    72,    73,    74,    74,
      75,    75,    77,    76,    79,    78,    81,    80,    83,    82,
      84,    85,    86,    87,    88,    88,    89,    89,    90,    90,
      91,    91,    91,    91,    91,    91,    92,    92,    93,    93,
      93,    93
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     0,     4,     0,     4,     0,     4,     2,
       2,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     3,     3,
       0,     4,     0,     4,     3,     3,     3,     3,     1,     1,
       1,     1,     0,     4,     0,     4,     0,     4,     0,     4,
       3,     3,     3,     3,     1,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,    22,     0,     0,     0,
       3,     4,     5,     6,     7,     9,    10,     8,     0,    11,
      13,    15,     0,    19,    20,    17,     0,     0,     0,     0,
      24,    27,    28,    29,    30,    31,    33,    34,    35,    36,
      40,    41,    38,    39,   109,   108,   110,   111,    42,    32,
      37,    21,    26,    25,    23,    43,     0,    44,    46,    12,
      14,    16,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    81,    80,    45,     0,     0,
       0,     0,     0,    65,    47,    48,    50,    49,    57,    58,
      60,    51,    52,    56,     0,    53,    54,    55,    59,    61,
      62,    63,    64,     0,    66,     0,    82,    86,    84,    70,
       0,     0,     0,     0,    88,     0,    72,     0,     0,     0,
      74,     0,    96,    75,     0,     0,     0,     0,    76,    94,
      95,    90,    91,    92,     0,    93,     0,    68,    69,    78,
      79,    77,   104,   100,   103,   101,   102,   105,    67,    98,
      97,    83,   106,    87,    85,    71,    89,    73,    99,   107
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,    11,    27,    12,    28,    13,    29,    14,
      56,    15,    16,    17,    22,    54,    19,    59,    63,    84,
      85,   121,    86,    87,    88,   127,    89,   136,    90,    91,
      92,    93,   141,    94,    95,   124,    96,   126,    97,   125,
      98,   134,    99,   100,   101,   102,   131,   123,   148,   149,
     151,   152
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -146
static const yytype_int16 yypact[] =
{
    -146,     1,  -146,   -14,   -14,   -14,  -146,   -16,     9,   -14,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,    23,  -146,
    -146,  -146,    47,  -146,  -146,  -146,     5,    38,    38,    38,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,    38,  -146,  -146,  -146,
    -146,  -146,  -146,    11,    25,    43,    46,    50,    69,    70,
      71,    72,    73,    74,    75,  -146,  -146,  -146,    76,    77,
      78,    79,    80,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,    81,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,    61,  -146,    87,  -146,  -146,  -146,  -146,
      86,    33,    33,    33,  -146,    33,  -146,    89,    61,    55,
    -146,    67,  -146,    90,    61,    61,    61,    67,  -146,  -146,
    -146,  -146,  -146,  -146,    61,  -146,    67,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,    67,  -146,
    -146,    61,  -146,    61,    61,    67,    61,    67,  -146,  -146
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,    32,  -146,  -146,    24,   -20,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,   -99,  -146,  -125,  -145,
     -87,   -22
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      55,     2,   155,   158,     3,     4,     5,    23,    60,    61,
     158,   157,   158,   132,   133,    18,   135,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    20,    21,
      75,    76,    24,    25,    57,     6,    62,     7,   153,   154,
       8,     9,    77,    78,    79,     6,    26,   156,    80,    81,
     129,   130,    82,   103,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    58,    44,
      45,   104,    46,    47,   105,    48,   139,   140,   106,    49,
      50,   120,    51,    44,    45,    52,    46,    47,    53,   142,
     143,   144,   145,   146,   147,    83,   138,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     122,   128,   137,   150,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
       0,   159,   159,     0,   159
};

static const yytype_int16 yycheck[] =
{
      22,     0,   127,   148,     3,     4,     5,    23,    28,    29,
     155,   136,   157,   112,   113,    29,   115,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,     4,     5,
      19,    20,    23,     9,    29,    34,    56,    36,   125,   126,
      39,    40,    31,    32,    33,    34,    23,   134,    37,    38,
      17,    18,    41,    28,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    30,    22,
      23,    28,    25,    26,    28,    28,    21,    22,    28,    32,
      33,   103,    35,    22,    23,    38,    25,    26,    41,    22,
      23,    24,    25,    26,    27,    63,   118,    28,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    28,    28,
      23,    25,    23,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,   153,   154,    -1,   156
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    43,     0,     3,     4,     5,    34,    36,    39,    40,
      44,    45,    47,    49,    51,    53,    54,    55,    29,    58,
      58,    58,    56,    23,    23,    58,    23,    46,    48,    50,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    22,    23,    25,    26,    28,    32,
      33,    35,    38,    41,    57,    93,    52,    29,    30,    59,
      59,    59,    59,    60,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    19,    20,    31,    32,    33,
      37,    38,    41,    55,    61,    62,    64,    65,    66,    68,
      70,    71,    72,    73,    75,    76,    78,    80,    82,    84,
      85,    86,    87,    28,    28,    28,    28,    28,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    28,    28,
      93,    63,    23,    89,    77,    81,    79,    67,    25,    17,
      18,    88,    88,    88,    83,    88,    69,    23,    93,    21,
      22,    74,    22,    23,    24,    25,    26,    27,    90,    91,
      23,    92,    93,    92,    92,    90,    92,    90,    91,    93
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:

/* Line 1455 of yacc.c  */
#line 64 "rules.y"
    {
               current_target = target_entry_new(TYPE_PROGRAM, (yyvsp[(2) - (2)]));
           }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 68 "rules.y"
    {
               module_add_target(current_target);
               current_target = NULL;
           }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 75 "rules.y"
    {
               current_target = target_entry_new(TYPE_LIBRARY, (yyvsp[(2) - (2)]));
           }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 79 "rules.y"
    {
               module_add_target(current_target);
               current_target = NULL;
           }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 86 "rules.y"
    {
               current_target = target_entry_new(TYPE_WORKER, (yyvsp[(2) - (2)]));
           }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 90 "rules.y"
    {
               module_add_target(current_target);
               current_target = NULL;
           }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 97 "rules.y"
    {
               current_target = target_entry_new(TYPE_CMI, (yyvsp[(2) - (2)]));
       }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 101 "rules.y"
    {
           module_add_target(current_target);
           current_target = NULL;
       }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 106 "rules.y"
    { module_set_skip_condition((yyvsp[(2) - (2)])); }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 108 "rules.y"
    { check_namgen_version((yyvsp[(2) - (2)])); }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 143 "rules.y"
    {
              (yyval) = (yyvsp[(2) - (3)]);
           }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 160 "rules.y"
    { target_set_destdir_path(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 169 "rules.y"
    { target_set_skip_install(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 170 "rules.y"
    { target_set_skip_shared(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 171 "rules.y"
    { target_set_skip_static(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 172 "rules.y"
    { target_set_convencience(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 176 "rules.y"
    { src_gatherer_reset(); }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 177 "rules.y"
    { current_target->src = src_gatherer_get_result(); }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 181 "rules.y"
    { current_target->src_ext = (yyvsp[(3) - (3)]); }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 185 "rules.y"
    { current_target->src_ext = (yyvsp[(3) - (3)]); }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 188 "rules.y"
    { src_gatherer_reset(); }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 189 "rules.y"
    { current_target->extra_obj = src_gatherer_get_result(); }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 192 "rules.y"
    { src_gatherer_reset(); }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 193 "rules.y"
    { current_target->extra_clean = src_gatherer_get_result(); }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 197 "rules.y"
    { current_target->cmd = (yyvsp[(3) - (3)]); }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 205 "rules.y"
    {
                     (yyval) = (yyvsp[(3) - (3)]);
                 }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 211 "rules.y"
    { current_target->lib_version_num = strdup((yyvsp[(3) - (3)])); }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 216 "rules.y"
    { current_target->other_flags &= ~VERSION_INFO_SCHEMA; }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 217 "rules.y"
    { current_target->other_flags |= VERSION_INFO_SCHEMA; }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 220 "rules.y"
    { sb_reset(sb); }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 221 "rules.y"
    { current_target->compile_flags = sb_make_cstring(sb); }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 224 "rules.y"
    { sb_reset(sb); }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 225 "rules.y"
    { current_target->link_flags = sb_make_cstring(sb); }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 228 "rules.y"
    { sb_reset(sb); }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 229 "rules.y"
    { current_target->libs = sb_make_cstring(sb); }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 232 "rules.y"
    { sb_reset(sb); }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 233 "rules.y"
    { current_target->export_include = sb_make_cstring(sb); }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 237 "rules.y"
    { (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 241 "rules.y"
    { (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 245 "rules.y"
    { (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 249 "rules.y"
    { (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 252 "rules.y"
    { (yyval) = "true"; }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 253 "rules.y"
    { (yyval) = "false"; }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 257 "rules.y"
    { target_add_dependency(current_target, (yyvsp[(1) - (1)])); }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 259 "rules.y"
    { target_add_dependency(current_target, (yyvsp[(2) - (2)])); }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 267 "rules.y"
    { src_gatherer_add_file((yyvsp[(1) - (1)])); }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 268 "rules.y"
    { src_gatherer_add_file((yyvsp[(1) - (1)])); }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 269 "rules.y"
    { src_gatherer_add_file((yyvsp[(1) - (1)])); }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 270 "rules.y"
    { src_gatherer_add_wildcard((yyvsp[(1) - (1)])); }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 271 "rules.y"
    { src_gatherer_add_variable((yyvsp[(1) - (1)])); }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 272 "rules.y"
    { src_gather_set_explicit(); }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 276 "rules.y"
    { sb_append_str(sb, (yyvsp[(1) - (1)])); sb_append_str(sb, " "); }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 278 "rules.y"
    { sb_append_str(sb, (yyvsp[(2) - (2)])); sb_append_str(sb, " "); }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 281 "rules.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 282 "rules.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 283 "rules.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 284 "rules.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;



/* Line 1455 of yacc.c  */
#line 2022 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



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
		      yytoken, &yylval);
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



