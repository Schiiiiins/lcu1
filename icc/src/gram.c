
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
#line 1 "../src/gram.y"
 static const char gram_y[] =
"@(#)$Id: gram.y,v 1.28 2008/06/26 14:46:43 jw Exp $";
/********************************************************************
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License or the Artistic License, as specified in the README file.
 *
 *  For more information about this program, or for information on how
 *  to contact the author, see the README file or <ic@je-wulff.de>
 *
 *	ANSI C Grammar
 *
 *	The ANSI C scanner/parser was posted on the net by Jeff Lee,
 *	in 1985. His net address is:
 *		jeff%gatech.CSNet@CSNet-Relay.ARPA, or jeff@gatech
 *	There was no open source license I could find.
 *
 *	Acknowledgement: Jeff Lee and the draft ANSI C standard grammar.
 *	Kernighan and Ritchie "The C Programming Language" 2nd Ed. A13.
 *
 *	This grammar was expanded to support most of the latest extensions
 *	used in gcc.
 *
 *	gram.y
 *	C grammar for immcc compiler
 *
 *******************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<stdarg.h>
#include	<string.h>

#include	"icc.h"
#include	"comp.h"

#define ENDSTACKSIZE	100
// #define LEAS		2
#define LEAI		170
#define LARGE		(~0U>>2)
static const char	idOp[] = "+-+-";	/* increment/decrement operator selection */

static Symbol		typedefSymbol = { "typedef", UDF, UDFA, };

#ifndef LMAIN
typedef struct LineEntry {
    unsigned int	pStart;
    unsigned int	vStart;
    unsigned int	equOp;
    unsigned int	vEnd;
    unsigned int	pEnd;
    Symbol *		sp;
    int			ppIdx;
} LineEntry;

static LineEntry *	lineEntryArray = NULL;
static LineEntry *	lep = NULL;		/* associated array allocated from heap */
#ifdef LEAS
static unsigned int	udfCount = LEAS + 2;	/* start with guard value space */
#else
static unsigned int	udfCount = LEAI;	/* 170 is approx 4 kB */
#endif
static unsigned int	endStack[ENDSTACKSIZE];
static unsigned int *	esp = endStack;

static void		immVarFound(unsigned int start, unsigned int end, Symbol* sp);
static void		immVarRemove(unsigned int start, unsigned int end, Symbol* sp);
static void		immAssignFound(unsigned int start, unsigned int operator,
			    unsigned int end, Symbol* sp, int ppi);
static unsigned int	pushEndStack(unsigned int value);
static unsigned int	popEndStack(void);
#else	/* LMAIN */
static void		yyerror(char *s, ...);
#endif	/* LMAIN */


/* Line 189 of yacc.c  */
#line 151 "gram.c"

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
     IDENTIFIER = 258,
     IMM_IDENTIFIER = 259,
     CONSTANT = 260,
     STRING_LITERAL = 261,
     SIZEOF = 262,
     PTR_OP = 263,
     INC_OP = 264,
     DEC_OP = 265,
     LEFT_OP = 266,
     RIGHT_OP = 267,
     LE_OP = 268,
     GE_OP = 269,
     EQ_OP = 270,
     NE_OP = 271,
     AND_OP = 272,
     OR_OP = 273,
     MUL_ASSIGN = 274,
     DIV_ASSIGN = 275,
     MOD_ASSIGN = 276,
     ADD_ASSIGN = 277,
     SUB_ASSIGN = 278,
     LEFT_ASSIGN = 279,
     RIGHT_ASSIGN = 280,
     AND_ASSIGN = 281,
     XOR_ASSIGN = 282,
     OR_ASSIGN = 283,
     TYPE_NAME = 284,
     TYPEDEF = 285,
     TYPEOF = 286,
     EXTERN = 287,
     STATIC = 288,
     AUTO = 289,
     REGISTER = 290,
     CHAR = 291,
     SHORT = 292,
     INT = 293,
     LONG = 294,
     SIGNED = 295,
     UNSIGNED = 296,
     FLOAT = 297,
     DOUBLE = 298,
     CONST = 299,
     VOLATILE = 300,
     VOID = 301,
     STRUCT = 302,
     UNION = 303,
     ENUM = 304,
     ELIPSIS = 305,
     CASE = 306,
     DEFAULT = 307,
     IF = 308,
     ELSE = 309,
     SWITCH = 310,
     WHILE = 311,
     DO = 312,
     FOR = 313,
     GOTO = 314,
     CONTINUE = 315,
     BREAK = 316,
     RETURN = 317
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define IMM_IDENTIFIER 259
#define CONSTANT 260
#define STRING_LITERAL 261
#define SIZEOF 262
#define PTR_OP 263
#define INC_OP 264
#define DEC_OP 265
#define LEFT_OP 266
#define RIGHT_OP 267
#define LE_OP 268
#define GE_OP 269
#define EQ_OP 270
#define NE_OP 271
#define AND_OP 272
#define OR_OP 273
#define MUL_ASSIGN 274
#define DIV_ASSIGN 275
#define MOD_ASSIGN 276
#define ADD_ASSIGN 277
#define SUB_ASSIGN 278
#define LEFT_ASSIGN 279
#define RIGHT_ASSIGN 280
#define AND_ASSIGN 281
#define XOR_ASSIGN 282
#define OR_ASSIGN 283
#define TYPE_NAME 284
#define TYPEDEF 285
#define TYPEOF 286
#define EXTERN 287
#define STATIC 288
#define AUTO 289
#define REGISTER 290
#define CHAR 291
#define SHORT 292
#define INT 293
#define LONG 294
#define SIGNED 295
#define UNSIGNED 296
#define FLOAT 297
#define DOUBLE 298
#define CONST 299
#define VOLATILE 300
#define VOID 301
#define STRUCT 302
#define UNION 303
#define ENUM 304
#define ELIPSIS 305
#define CASE 306
#define DEFAULT 307
#define IF 308
#define ELSE 309
#define SWITCH 310
#define WHILE 311
#define DO 312
#define FOR 313
#define GOTO 314
#define CONTINUE 315
#define BREAK 316
#define RETURN 317




#if ! defined c_STYPE && ! defined c_STYPE_IS_DECLARED
typedef union c_STYPE
{

/* Line 214 of yacc.c  */
#line 77 "../src/gram.y"
					/* stack type */
    Token		tok;



/* Line 214 of yacc.c  */
#line 317 "gram.c"
} c_STYPE;
# define c_STYPE_IS_TRIVIAL 1
# define yystype c_STYPE /* obsolescent; will be withdrawn */
# define c_STYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 329 "gram.c"

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
	 || (defined c_STYPE_IS_TRIVIAL && c_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 c_ss_alloc;
  c_STYPE c_vs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (c_STYPE)) \
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
#define YYLAST   1693

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  278
/* YYNRULES -- Number of states.  */
#define YYNSTATES  482

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    78,     2,     2,     2,    85,    73,     2,
      65,    66,    74,    75,    72,    76,    86,    84,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    70,    71,
      82,    69,    83,    79,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,     2,    67,    81,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,    80,    68,    77,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    14,    15,    20,
      22,    25,    28,    32,    36,    38,    41,    43,    46,    48,
      50,    52,    54,    56,    58,    60,    62,    64,    66,    68,
      70,    72,    74,    76,    78,    80,    85,    91,    96,   102,
     104,   106,   108,   113,   119,   121,   124,   127,   130,   132,
     134,   135,   138,   140,   144,   146,   150,   153,   157,   161,
     163,   166,   168,   171,   173,   177,   179,   182,   186,   191,
     197,   203,   210,   213,   215,   218,   220,   224,   226,   230,
     232,   235,   237,   239,   243,   248,   252,   257,   262,   268,
     271,   275,   279,   284,   288,   293,   296,   298,   301,   304,
     308,   310,   313,   315,   319,   321,   325,   327,   331,   334,
     336,   338,   342,   344,   348,   353,   358,   364,   366,   370,
     372,   375,   377,   379,   382,   386,   391,   394,   398,   402,
     407,   411,   416,   421,   427,   430,   434,   438,   443,   447,
     452,   457,   463,   465,   467,   469,   471,   473,   475,   479,
     484,   488,   490,   493,   497,   500,   504,   508,   513,   517,
     522,   527,   533,   535,   538,   544,   552,   558,   564,   572,
     579,   587,   595,   604,   612,   621,   630,   640,   644,   647,
     650,   653,   657,   662,   664,   668,   670,   674,   678,   680,
     682,   684,   686,   688,   690,   692,   694,   696,   698,   700,
     702,   708,   713,   715,   717,   721,   723,   727,   729,   733,
     735,   739,   741,   745,   747,   751,   755,   757,   761,   765,
     769,   773,   775,   779,   783,   785,   789,   793,   795,   799,
     803,   807,   809,   811,   816,   822,   824,   827,   830,   833,
     836,   841,   847,   849,   852,   855,   858,   860,   862,   864,
     866,   868,   870,   872,   877,   883,   887,   892,   897,   903,
     907,   911,   914,   917,   919,   922,   925,   927,   929,   931,
     935,   940,   944,   949,   951,   955,   957,   960,   962
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      88,     0,    -1,    -1,    88,    89,    -1,    90,    -1,    93,
      -1,   113,    92,    -1,    -1,    95,   113,    91,    92,    -1,
     131,    -1,    94,   131,    -1,    95,    71,    -1,    95,   103,
      71,    -1,    95,     1,    71,    -1,    93,    -1,    94,    93,
      -1,   106,    -1,    96,   106,    -1,    34,    -1,    33,    -1,
      32,    -1,    30,    -1,    46,    -1,    35,    -1,    36,    -1,
      37,    -1,    38,    -1,    39,    -1,    42,    -1,    43,    -1,
      40,    -1,    41,    -1,    99,    -1,   109,    -1,    29,    -1,
      31,    65,   152,    66,    -1,    31,    65,   152,     1,    66,
      -1,    31,    65,   125,    66,    -1,    31,    65,   125,     1,
      66,    -1,    44,    -1,    45,    -1,   100,    -1,   100,    63,
     102,    68,    -1,   100,    63,   102,     1,    68,    -1,   101,
      -1,   101,     3,    -1,   101,    29,    -1,   101,   160,    -1,
      47,    -1,    48,    -1,    -1,   102,   105,    -1,   104,    -1,
     103,    72,   104,    -1,   113,    -1,   113,    69,   123,    -1,
     106,    71,    -1,   106,   107,    71,    -1,   106,     1,    71,
      -1,    97,    -1,    97,   106,    -1,    98,    -1,    98,   106,
      -1,   108,    -1,   107,    72,   108,    -1,   113,    -1,    70,
     140,    -1,   113,    70,   140,    -1,    49,    63,   110,    68,
      -1,    49,    63,   110,     1,    68,    -1,    49,     3,    63,
     110,    68,    -1,    49,     3,    63,   110,     1,    68,    -1,
      49,     3,    -1,   111,    -1,   111,    72,    -1,   112,    -1,
     111,    72,   112,    -1,     3,    -1,     3,    69,   140,    -1,
     114,    -1,   116,   114,    -1,     3,    -1,   160,    -1,    65,
     113,    66,    -1,    65,   113,     1,    66,    -1,   114,    64,
      67,    -1,   114,    64,     1,    67,    -1,   114,    64,   140,
      67,    -1,   114,    64,   140,     1,    67,    -1,   115,    66,
      -1,   115,     1,    66,    -1,   115,   118,    66,    -1,   115,
     118,     1,    66,    -1,   115,   119,    66,    -1,   115,   119,
       1,    66,    -1,   114,    65,    -1,    74,    -1,    74,   117,
      -1,    74,   116,    -1,    74,   117,   116,    -1,    98,    -1,
     117,    98,    -1,   120,    -1,   120,    72,    50,    -1,   122,
      -1,   122,    72,    50,    -1,   121,    -1,   120,    72,   121,
      -1,   106,   113,    -1,   125,    -1,     3,    -1,   122,    72,
       3,    -1,   137,    -1,    63,   124,    68,    -1,    63,   124,
       1,    68,    -1,    63,   124,    72,    68,    -1,    63,   124,
       1,    72,    68,    -1,   123,    -1,   124,    72,   123,    -1,
     106,    -1,   106,   126,    -1,   116,    -1,   127,    -1,   116,
     127,    -1,    65,   126,    66,    -1,    65,   126,     1,    66,
      -1,    64,    67,    -1,    64,     1,    67,    -1,    64,   140,
      67,    -1,    64,   140,     1,    67,    -1,   127,    64,    67,
      -1,   127,    64,     1,    67,    -1,   127,    64,   140,    67,
      -1,   127,    64,   140,     1,    67,    -1,    65,    66,    -1,
      65,     1,    66,    -1,    65,   118,    66,    -1,    65,   118,
       1,    66,    -1,   127,    65,    66,    -1,   127,    65,     1,
      66,    -1,   127,    65,   118,    66,    -1,   127,    65,   118,
       1,    66,    -1,   129,    -1,   130,    -1,   131,    -1,   133,
      -1,   134,    -1,   135,    -1,     3,    70,   128,    -1,    51,
     140,    70,   128,    -1,    52,    70,   128,    -1,    71,    -1,
     136,    71,    -1,   136,     1,    71,    -1,    63,    68,    -1,
      63,     1,    68,    -1,    63,   132,    68,    -1,    63,   132,
       1,    68,    -1,    63,    94,    68,    -1,    63,    94,     1,
      68,    -1,    63,    94,   132,    68,    -1,    63,    94,   132,
       1,    68,    -1,   128,    -1,   132,   128,    -1,    53,    65,
     136,    66,   128,    -1,    53,    65,   136,    66,   128,    54,
     128,    -1,    55,    65,   136,    66,   128,    -1,    56,    65,
     136,    66,   128,    -1,    57,   128,    56,    65,   136,    66,
      71,    -1,    58,    65,    71,    71,    66,   128,    -1,    58,
      65,    71,    71,   136,    66,   128,    -1,    58,    65,    71,
     136,    71,    66,   128,    -1,    58,    65,    71,   136,    71,
     136,    66,   128,    -1,    58,    65,   136,    71,    71,    66,
     128,    -1,    58,    65,   136,    71,    71,   136,    66,   128,
      -1,    58,    65,   136,    71,   136,    71,    66,   128,    -1,
      58,    65,   136,    71,   136,    71,   136,    66,   128,    -1,
      59,     3,    71,    -1,    60,    71,    -1,    61,    71,    -1,
      62,    71,    -1,    62,   136,    71,    -1,    62,   136,     1,
      71,    -1,   137,    -1,   136,    72,   137,    -1,   139,    -1,
     152,   138,   137,    -1,   160,   138,   137,    -1,    69,    -1,
      19,    -1,    20,    -1,    21,    -1,    22,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,
     141,    -1,   141,    79,   141,    70,   139,    -1,   141,    79,
      70,   139,    -1,   139,    -1,   142,    -1,   141,    18,   142,
      -1,   143,    -1,   142,    17,   143,    -1,   144,    -1,   143,
      80,   144,    -1,   145,    -1,   144,    81,   145,    -1,   146,
      -1,   145,    73,   146,    -1,   147,    -1,   146,    15,   147,
      -1,   146,    16,   147,    -1,   148,    -1,   147,    82,   148,
      -1,   147,    83,   148,    -1,   147,    13,   148,    -1,   147,
      14,   148,    -1,   149,    -1,   148,    11,   149,    -1,   148,
      12,   149,    -1,   150,    -1,   149,    75,   150,    -1,   149,
      76,   150,    -1,   151,    -1,   150,    74,   151,    -1,   150,
      84,   151,    -1,   150,    85,   151,    -1,   152,    -1,   153,
      -1,    65,   125,    66,   151,    -1,    65,   125,     1,    66,
     151,    -1,   155,    -1,     9,   152,    -1,    10,   152,    -1,
     154,   151,    -1,     7,   152,    -1,     7,    65,   125,    66,
      -1,     7,    65,   125,     1,    66,    -1,   156,    -1,     9,
     153,    -1,    10,   153,    -1,     7,   153,    -1,    73,    -1,
      74,    -1,    75,    -1,    76,    -1,    77,    -1,    78,    -1,
     157,    -1,   155,    64,   136,    67,    -1,   155,    64,   136,
       1,    67,    -1,   155,    65,    66,    -1,   155,    65,     1,
      66,    -1,   155,    65,   158,    66,    -1,   155,    65,   158,
       1,    66,    -1,   155,    86,     3,    -1,   155,     8,     3,
      -1,   155,     9,    -1,   155,    10,    -1,   160,    -1,   156,
       9,    -1,   156,    10,    -1,     3,    -1,     5,    -1,   159,
      -1,    65,   136,    66,    -1,    65,   136,     1,    66,    -1,
      65,   131,    66,    -1,    65,   131,     1,    66,    -1,   137,
      -1,   158,    72,   137,    -1,     6,    -1,   159,     6,    -1,
       4,    -1,    65,   160,    66,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   128,   128,   133,   141,   146,   154,   163,   163,   179,
     184,   192,   200,   227,   239,   244,   252,   262,   275,   280,
     285,   290,   298,   303,   308,   313,   318,   323,   328,   333,
     338,   343,   348,   353,   358,   363,   368,   374,   379,   388,
     393,   401,   406,   411,   420,   425,   432,   438,   446,   451,
     459,   464,   472,   477,   493,   498,   506,   511,   516,   525,
     530,   535,   540,   548,   553,   561,   567,   572,   581,   586,
     592,   598,   605,   614,   619,   627,   632,   640,   646,   655,
     671,   690,   695,   700,   705,   711,   716,   722,   727,   733,
     741,   750,   758,   767,   775,   787,   798,   803,   808,   813,
     821,   826,   834,   839,   847,   852,   860,   865,   873,   879,
     887,   893,   902,   907,   912,   918,   923,   932,   937,   945,
     950,   958,   963,   968,   976,   981,   987,   992,   998,  1003,
    1009,  1014,  1020,  1025,  1031,  1036,  1042,  1047,  1053,  1058,
    1064,  1069,  1078,  1083,  1088,  1093,  1098,  1103,  1111,  1117,
    1122,  1130,  1135,  1140,  1149,  1154,  1160,  1165,  1171,  1176,
    1182,  1187,  1196,  1201,  1209,  1214,  1219,  1227,  1232,  1237,
    1242,  1247,  1252,  1257,  1262,  1267,  1272,  1280,  1286,  1291,
    1296,  1301,  1306,  1315,  1320,  1328,  1334,  1340,  1362,  1367,
    1372,  1377,  1382,  1387,  1392,  1397,  1402,  1407,  1412,  1420,
    1425,  1431,  1445,  1453,  1458,  1466,  1471,  1479,  1484,  1492,
    1497,  1505,  1510,  1518,  1523,  1528,  1536,  1541,  1546,  1551,
    1556,  1564,  1569,  1574,  1582,  1587,  1592,  1600,  1605,  1610,
    1615,  1623,  1628,  1633,  1638,  1647,  1652,  1657,  1662,  1667,
    1672,  1677,  1686,  1691,  1710,  1729,  1737,  1742,  1747,  1752,
    1757,  1762,  1770,  1775,  1780,  1786,  1791,  1797,  1802,  1808,
    1814,  1820,  1825,  1833,  1838,  1857,  1879,  1885,  1890,  1895,
    1900,  1906,  1911,  1920,  1925,  1933,  1938,  1946,  1965
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "IMM_IDENTIFIER",
  "CONSTANT", "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP",
  "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP",
  "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN",
  "OR_ASSIGN", "TYPE_NAME", "TYPEDEF", "TYPEOF", "EXTERN", "STATIC",
  "AUTO", "REGISTER", "CHAR", "SHORT", "INT", "LONG", "SIGNED", "UNSIGNED",
  "FLOAT", "DOUBLE", "CONST", "VOLATILE", "VOID", "STRUCT", "UNION",
  "ENUM", "ELIPSIS", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE",
  "DO", "FOR", "GOTO", "CONTINUE", "BREAK", "RETURN", "'{'", "'['", "'('",
  "')'", "']'", "'}'", "'='", "':'", "';'", "','", "'&'", "'*'", "'+'",
  "'-'", "'~'", "'!'", "'?'", "'|'", "'^'", "'<'", "'>'", "'/'", "'%'",
  "'.'", "$accept", "translation_unit", "external_declaration",
  "function_definition", "$@1", "function_body", "declaration",
  "declaration_list", "declaration_specifiers", "storage_class_specifier",
  "type_specifier", "type_qualifier", "struct_or_union_specifier",
  "struct_or_union_tag", "struct_or_union", "struct_declaration_list",
  "init_declarator_list", "init_declarator", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list_comma",
  "enumerator_list", "enumerator", "declarator", "direct_declarator",
  "parameter_head", "pointer", "type_qualifier_list",
  "parameter_type_list", "parameter_identifier_list", "parameter_list",
  "parameter_declaration", "identifier_list", "initializer",
  "initializer_list", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "statement", "labeled_statement",
  "expression_statement", "compound_statement", "statement_list",
  "selection_statement", "iteration_statement", "jump_statement",
  "expression", "assignment_expression", "assignment_operator",
  "conditional_expression", "constant_expression", "logical_OR_expression",
  "logical_AND_expression", "inclusive_OR_expression",
  "exclusive_OR_expression", "AND_expression", "equality_expression",
  "relational_expression", "shift_expression", "additive_expression",
  "multiplicative_expression", "cast_expression", "unary_expression",
  "imm_unary_expression", "unary_operator", "postfix_expression",
  "imm_postfix_expression", "primary_expression", "argument_expr_list",
  "string_literal", "imm_identifier", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   123,    91,    40,    41,    93,   125,    61,
      58,    59,    44,    38,    42,    43,    45,   126,    33,    63,
     124,    94,    60,    62,    47,    37,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    88,    89,    89,    90,    91,    90,    92,
      92,    93,    93,    93,    94,    94,    95,    95,    96,    96,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    98,
      98,    99,    99,    99,   100,   100,   100,   100,   101,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   105,   106,
     106,   106,   106,   107,   107,   108,   108,   108,   109,   109,
     109,   109,   109,   110,   110,   111,   111,   112,   112,   113,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   116,   116,   116,   116,
     117,   117,   118,   118,   119,   119,   120,   120,   121,   121,
     122,   122,   123,   123,   123,   123,   123,   124,   124,   125,
     125,   126,   126,   126,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   128,   128,   128,   128,   129,   129,
     129,   130,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   132,   132,   133,   133,   133,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   135,   135,   135,
     135,   135,   135,   136,   136,   137,   137,   137,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   139,
     139,   139,   140,   141,   141,   142,   142,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   147,   147,   147,   147,
     147,   148,   148,   148,   149,   149,   149,   150,   150,   150,
     150,   151,   151,   151,   151,   152,   152,   152,   152,   152,
     152,   152,   153,   153,   153,   153,   154,   154,   154,   154,
     154,   154,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   156,   156,   156,   157,   157,   157,   157,
     157,   157,   157,   158,   158,   159,   159,   160,   160
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     2,     0,     4,     1,
       2,     2,     3,     3,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     5,     4,     5,     1,
       1,     1,     4,     5,     1,     2,     2,     2,     1,     1,
       0,     2,     1,     3,     1,     3,     2,     3,     3,     1,
       2,     1,     2,     1,     3,     1,     2,     3,     4,     5,
       5,     6,     2,     1,     2,     1,     3,     1,     3,     1,
       2,     1,     1,     3,     4,     3,     4,     4,     5,     2,
       3,     3,     4,     3,     4,     2,     1,     2,     2,     3,
       1,     2,     1,     3,     1,     3,     1,     3,     2,     1,
       1,     3,     1,     3,     4,     4,     5,     1,     3,     1,
       2,     1,     1,     2,     3,     4,     2,     3,     3,     4,
       3,     4,     4,     5,     2,     3,     3,     4,     3,     4,
       4,     5,     1,     1,     1,     1,     1,     1,     3,     4,
       3,     1,     2,     3,     2,     3,     3,     4,     3,     4,
       4,     5,     1,     2,     5,     7,     5,     5,     7,     6,
       7,     7,     8,     7,     8,     8,     9,     3,     2,     2,
       2,     3,     4,     1,     3,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     4,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     1,     4,     5,     1,     2,     2,     2,     2,
       4,     5,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     5,     3,     4,     4,     5,     3,
       3,     2,     2,     1,     2,     2,     1,     1,     1,     3,
       4,     3,     4,     1,     3,     1,     2,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    81,   277,    34,    21,     0,    20,    19,
      18,    23,    24,    25,    26,    27,    30,    31,    28,    29,
      39,    40,    22,    48,    49,     0,     0,    96,     3,     4,
       5,     0,     0,    59,    61,    32,    41,    44,    16,    33,
       0,    79,     0,     0,    82,     0,    72,     0,     0,    82,
     100,    98,    97,     0,    11,     0,    52,     7,    17,    60,
      62,    50,    45,    46,     0,    47,     0,     6,    14,     0,
       0,     9,     0,    95,     0,   110,    89,   119,     0,     0,
     102,   106,   104,   109,    80,   266,   267,   275,     0,     0,
       0,     0,   246,   247,   248,   249,   250,   251,   119,     0,
       0,     0,   235,   252,   268,     0,    77,     0,    73,    75,
       0,    83,   278,   101,    99,    13,    12,     0,     0,     0,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     151,     0,   162,   142,   143,   144,     0,   145,   146,   147,
       0,   183,   185,   199,   203,   205,   207,   209,   211,   213,
     216,   221,   224,   227,   231,   232,   242,   263,    15,    10,
      54,     0,    85,   202,     0,   231,   263,    90,     0,     0,
     108,   121,   120,   122,     0,    91,     0,    93,     0,     0,
       0,   239,   236,   237,     0,     0,     0,   121,     0,    37,
       0,    35,   238,     0,   261,   262,     0,     0,     0,   276,
       0,     0,     0,    68,    74,    84,    53,     0,    55,   112,
       8,     0,    42,    51,     0,   155,     0,     0,   245,     0,
     243,   244,     0,     0,     0,     0,     0,     0,     0,     0,
     178,   179,   180,     0,     0,   263,     0,   158,     0,     0,
     156,   163,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   188,     0,   264,   265,     0,    86,
       0,    87,     0,   126,     0,     0,   134,     0,     0,   123,
       0,     0,    92,    94,   103,   107,   111,   105,     0,     0,
     271,     0,   269,    38,    36,   260,     0,     0,   255,   273,
       0,   259,     0,    70,    78,    69,    76,   117,     0,    43,
       0,     0,    56,     0,    63,    65,   148,     0,   150,     0,
       0,     0,     0,     0,     0,   177,     0,   181,     0,     0,
     159,     0,   160,   157,   153,   184,   204,     0,     0,   206,
     208,   210,   212,   214,   215,   219,   220,   217,   218,   222,
     223,   225,   226,   228,   229,   230,   186,   187,    88,   127,
       0,   128,   135,     0,   136,     0,   124,     0,   130,     0,
       0,   138,     0,     0,   240,   272,   270,     0,   253,   256,
       0,   257,     0,    71,     0,   113,     0,    58,    66,    57,
       0,     0,   149,     0,     0,     0,     0,     0,     0,     0,
     182,     0,   233,   161,   201,     0,   129,   137,   125,   131,
       0,   132,   139,     0,   140,   241,   254,   258,   274,   114,
       0,   115,   118,    64,    67,   164,   166,   167,     0,     0,
       0,     0,     0,     0,   234,   200,   133,   141,   116,     0,
       0,   169,     0,     0,     0,     0,     0,     0,   165,   168,
     170,   171,     0,   173,     0,     0,     0,   172,   174,   175,
       0,   176
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    28,    29,   119,    67,    68,    69,    70,    32,
      33,    34,    35,    36,    37,   120,    55,    56,   223,    38,
     333,   334,    39,   107,   108,   109,    48,    41,    42,    43,
      52,   297,    79,    80,    81,    82,   218,   328,    83,   182,
     183,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   288,   152,   174,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   101,   102,   166,
     103,   320,   104,   167
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -213
static const yytype_int16 yypact[] =
{
    -213,  1376,  -213,  -213,  -213,  -213,  -213,   -50,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,    91,    47,   246,  -213,  -213,
    -213,    82,  1644,  1644,  1644,  -213,   -36,   293,  -213,  -213,
    1593,   -20,  1523,   221,  -213,   857,   -10,    15,    53,    -8,
    -213,  -213,   246,   -16,  -213,   234,  -213,   307,  -213,  -213,
    -213,  -213,  -213,  -213,   134,  -213,   441,  -213,  -213,  1593,
      82,  -213,   390,  -213,    -5,  -213,  -213,    25,    56,    61,
      19,  -213,    24,  -213,   -20,  -213,  -213,  -213,   308,  1326,
    1326,   933,  -213,  -213,  -213,  -213,  -213,  -213,   107,    94,
     131,  1236,    23,  -213,   161,    15,    67,     7,    86,  -213,
     110,  -213,  -213,  -213,  -213,  -213,  -213,    47,   980,  1593,
    1324,    -8,   117,   126,  1265,  1312,  1312,  1236,   128,   122,
     140,   188,   904,   215,   209,   158,   207,  1009,   807,  -213,
    -213,   519,  -213,  -213,  -213,  -213,   597,  -213,  -213,  -213,
      39,  -213,  -213,   152,   204,   202,   243,   260,   353,   136,
     378,   336,   218,  -213,   514,  -213,   405,   514,  -213,  -213,
     259,   277,  -213,  -213,     1,  -213,  -213,  -213,   703,  1425,
    -213,   285,  -213,   352,   270,  -213,   281,  -213,  1622,     6,
     807,  -213,  -213,  -213,   147,    63,  1474,   354,   287,  -213,
     291,  -213,  -213,   348,  -213,  -213,  1236,   717,   356,  -213,
      16,  1236,   302,  -213,    15,  -213,  -213,   980,  -213,  -213,
    -213,   319,  -213,  -213,   130,  -213,   904,   807,  -213,   933,
    -213,  -213,   305,   904,  1236,  1236,  1236,   345,  1023,   290,
    -213,  -213,  -213,    71,   150,   595,   340,  -213,   625,   363,
    -213,  -213,   366,  -213,  1236,  1236,  1056,  1236,  1236,  1236,
    1236,  1236,  1236,  1236,  1236,  1236,  1236,  1236,  1236,  1236,
    1236,  1236,  1236,  1236,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  1236,  -213,  -213,  1236,  -213,
     358,  -213,   374,  -213,    40,   377,  -213,   156,   164,   352,
     731,  1555,  -213,  -213,  -213,  -213,  -213,  -213,   213,   383,
    -213,   388,  -213,  -213,  -213,  -213,    10,   394,  -213,  -213,
     143,  -213,   393,  -213,  -213,  -213,  -213,  -213,    96,  -213,
     385,  1236,  -213,   355,  -213,   392,  -213,   904,  -213,   235,
     244,   253,   426,  1099,   357,  -213,   424,  -213,   439,  1236,
    -213,   440,  -213,  -213,  -213,  -213,   204,  1236,   -11,   202,
     243,   260,   353,   136,   136,   378,   378,   378,   378,   336,
     336,   218,   218,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
     443,  -213,  -213,   445,  -213,   447,  -213,   454,  -213,    51,
     461,  -213,   219,   464,  -213,  -213,  -213,   465,  -213,  -213,
     478,  -213,  1236,  -213,   -25,  -213,   755,  -213,  -213,  -213,
     137,  1236,  -213,   904,   904,   904,  1236,  1113,   362,  1132,
    -213,  1236,  -213,  -213,  -213,  1236,  -213,  -213,  -213,  -213,
     479,  -213,  -213,   481,  -213,  -213,  -213,  -213,  -213,  -213,
     463,  -213,  -213,  -213,  -213,   369,  -213,  -213,   255,   904,
     265,  1146,  1189,   364,  -213,  -213,  -213,  -213,  -213,   904,
     436,  -213,   904,   904,   266,   904,   269,  1222,  -213,  -213,
    -213,  -213,   904,  -213,   904,   904,   274,  -213,  -213,  -213,
     904,  -213
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,  -213,  -213,  -213,  -213,   450,     5,   507,   544,  -213,
    -213,   -14,  -213,  -213,  -213,  -213,  -213,   468,  -213,   242,
    -213,   176,  -213,   483,  -213,   375,     3,   -42,  -213,   -17,
    -213,   -28,  -213,  -213,   403,  -213,  -212,  -213,   -24,  -159,
    -158,  -120,  -213,  -213,   -21,   458,  -213,  -213,  -213,   -45,
    -102,   444,   -48,   -19,   349,   370,   367,   351,   368,   350,
     178,   101,   185,   189,   -98,   -23,   279,  -213,  -213,  -213,
    -213,  -213,  -213,    -1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -55
static const yytype_int16 c_table[] =
{
      44,    84,   290,   202,    40,   327,    30,   255,   212,   306,
      51,   397,   237,    50,    78,    45,   219,   322,   106,    71,
     298,    99,   100,   299,   173,    49,   251,    61,     3,     4,
      44,   203,   204,   205,    57,   114,    65,   298,   113,   299,
     252,   380,    44,   439,    72,    73,   195,   440,   169,   175,
       3,     4,   430,   105,   110,   115,   307,   184,   112,   425,
     181,   177,   186,   121,   311,   191,   192,   193,   291,    44,
     194,   176,   346,   170,   168,   213,    44,   398,   175,   173,
     180,   197,   254,    53,   323,     3,     4,   206,   207,   178,
     179,   188,   243,   195,    46,   198,   189,   404,    71,    27,
     176,   191,   192,   193,   175,   319,   336,   381,   232,   208,
     253,   254,    26,   338,   244,   219,    44,   194,   431,   111,
     170,    27,   185,   176,   176,   176,   176,   187,   251,   312,
     173,   330,   200,     3,     4,   254,   211,   245,     4,    84,
       3,     4,   347,   254,   400,   195,   168,    26,   309,   263,
     264,   348,   355,    54,    47,   175,    27,   383,   214,   294,
     199,   316,   181,   173,   405,   385,   308,   209,   406,   194,
     255,   178,   196,   373,   374,   375,   215,   176,    49,   197,
      44,    27,   195,   376,   195,   225,   377,   234,   175,   339,
     340,   341,   324,   344,   442,    26,   226,   201,   233,    64,
     331,   332,    26,   308,    27,   235,   194,   331,   194,   401,
     176,    27,   239,   310,   393,   402,   349,   412,   265,   266,
     433,   257,   384,    44,     3,     4,   245,   335,   245,   240,
     386,   256,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   422,   173,   236,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   392,    58,    59,    60,   175,   241,   394,
     238,   389,   258,   173,    77,   434,    26,    98,     3,     4,
      20,    21,   271,   445,   446,   447,    62,     4,   418,   176,
     438,   413,   272,   273,   219,   116,   117,   254,   175,   424,
     414,    85,   408,    86,    87,    88,   254,    89,    90,   415,
      27,   460,    63,   454,   259,   254,   175,   254,   118,   461,
     176,   462,   472,   260,   175,   474,   302,   254,   254,   468,
     480,   254,   470,   471,   289,   473,   254,   303,   176,   178,
     179,   315,   477,   313,   478,   479,   176,   314,    64,   321,
     481,   345,   224,   173,   365,   366,   367,   368,   261,   262,
     325,   448,   450,   190,   453,   337,   118,   455,   -54,   -54,
      98,    92,    93,    94,    95,    96,    97,   329,   175,   267,
     268,   171,   444,    85,     4,    86,    87,   124,   175,   125,
     126,   342,   175,   228,   230,   231,   464,   466,   350,    44,
     176,   269,   270,   335,   286,   287,   300,   301,   178,   196,
     176,    77,   476,   459,   176,   378,   409,   410,   419,   254,
      77,   353,    98,   451,   254,   467,   254,   354,    77,   363,
     364,   379,   122,   382,   123,     4,    86,    87,   124,   395,
     125,   126,   369,   370,   396,   138,   407,   172,   371,   372,
     399,   403,   411,    92,    93,    94,    95,    96,    97,    98,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,   416,   127,   128,   129,   420,   130,   131,   132,   133,
     134,   135,   136,   137,    66,   421,   138,   469,   423,   139,
     426,   427,   140,   428,    92,    93,    94,    95,    96,    97,
     246,   429,   123,     4,    86,    87,   124,   432,   125,   126,
     435,   458,   436,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,    77,   437,    31,   456,   457,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,   220,
     127,   128,   129,   141,   130,   131,   132,   133,   134,   135,
     136,   137,    66,   284,   138,   216,   443,   247,   210,   326,
     140,   305,    92,    93,    94,    95,    96,    97,   249,   248,
     123,     4,    86,    87,   124,   358,   125,   126,   285,   360,
     362,     0,     0,     0,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   359,   356,   351,   361,   123,     4,
      86,    87,   124,     0,   125,   126,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   127,   128,
     129,     0,   130,   131,   132,   133,   134,   135,   136,   137,
      66,   112,   138,     0,   284,   250,     0,     0,   140,     0,
      92,    93,    94,    95,    96,    97,   127,   128,   129,     0,
     130,   131,   132,   133,   134,   135,   136,   137,    66,     0,
     138,     0,     0,   352,     0,     0,   140,     0,    92,    93,
      94,    95,    96,    97,   292,     0,    85,     4,    86,    87,
     124,     0,   125,   126,     0,     0,     0,     0,   317,     0,
      85,     4,    86,    87,   124,     0,   125,   126,     0,     0,
       0,     0,   387,     0,    85,     4,    86,    87,   124,     0,
     125,   126,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,     4,
      86,    87,   124,     0,   125,   126,     0,     0,   138,     0,
     293,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,    97,   138,   318,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,    97,   138,     0,   388,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,    97,
      85,     4,    86,    87,   124,     0,   125,   126,   217,     0,
     138,     0,     0,   441,     0,     0,     0,     0,    92,    93,
      94,    95,    96,    97,     0,     0,     5,     0,     7,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,     0,     0,
      85,     0,    86,    87,    88,     0,    89,    90,     0,     0,
      66,     0,   138,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,    97,     5,     0,     7,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,   123,     4,    86,
      87,   124,     0,   125,   126,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,    97,    85,     4,    86,    87,
     124,     0,   125,   126,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   127,   128,   129,     0,   130,
     131,   132,   133,   134,   135,   136,   137,    66,     0,   138,
       0,     0,     0,     0,     0,   140,     0,    92,    93,    94,
      95,    96,    97,    85,     4,    86,    87,   124,     0,   125,
     126,     0,     0,     0,     0,     0,    66,     0,   138,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,    97,    85,     4,    86,    87,   124,     0,   125,   126,
       0,     0,     0,     0,     0,     0,    85,     4,    86,    87,
     124,     0,   125,   126,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   138,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,    97,    85,
       4,    86,    87,   124,     0,   125,   126,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
     242,     0,    92,    93,    94,    95,    96,    97,   138,     0,
       0,     0,     0,     0,   343,     0,    92,    93,    94,    95,
      96,    97,    85,     4,    86,    87,   124,     0,   125,   126,
       0,     0,     0,     0,     0,     0,    85,     4,    86,    87,
     124,   138,   125,   126,     0,     0,   357,     0,     0,    92,
      93,    94,    95,    96,    97,    85,     4,    86,    87,   124,
       0,   125,   126,     0,     0,     0,     0,     0,     0,    85,
       4,    86,    87,   124,     0,   125,   126,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
     417,     0,    92,    93,    94,    95,    96,    97,   138,   449,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,    97,    85,     4,    86,    87,   124,   138,   125,   126,
       0,     0,     0,   452,     0,    92,    93,    94,    95,    96,
      97,   138,   463,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,    97,    85,     4,    86,    87,   124,
       0,   125,   126,     0,     0,     0,     0,     0,     0,    85,
       4,    86,    87,   124,     0,   125,   126,     0,     0,     0,
       0,     0,     0,     0,   138,   465,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,    97,    85,     4,
      86,    87,   124,     0,   125,   126,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,   475,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
      97,   138,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,    97,    85,     4,    86,    87,   124,
       0,   125,   126,     0,     0,   221,     0,     0,     0,    85,
     227,    86,    87,    88,     0,    89,    90,     0,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     7,     0,     0,     0,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,     0,     2,   229,     0,     3,
       4,     0,     0,     0,     0,    92,    93,    94,    95,    96,
      97,    91,   222,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,    97,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,   295,     0,     3,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     5,     0,     7,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
     179,   296,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     5,     0,     7,     0,     0,     0,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    74,     0,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,   196,
     296,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     5,     0,     7,     0,   390,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     7,     0,     0,    76,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   391,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     7,     0,     0,    66,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,   304,     5,     0,     7,     0,     0,     0,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25
};

static const yytype_int16 c_check[] =
{
       1,    43,     1,   101,     1,   217,     1,    18,     1,     3,
      27,     1,   132,    27,    42,    65,   118,     1,     3,    40,
     179,    45,    45,   181,    72,    26,   146,    63,     3,     4,
      31,     8,     9,    10,    31,    52,    37,   196,    52,   197,
       1,     1,    43,    68,    64,    65,    91,    72,    69,    72,
       3,     4,     1,    63,     1,    71,    50,     1,    66,    70,
      77,    66,     1,    64,     1,    88,    89,    90,    67,    70,
      91,    72,     1,    70,    69,    68,    77,    67,   101,   127,
      77,    98,    72,     1,    68,     3,     4,    64,    65,    64,
      65,    72,   137,   138,     3,     1,    72,     1,   119,    74,
     101,   124,   125,   126,   127,   207,   226,    67,   127,    86,
      71,    72,    65,   233,   138,   217,   117,   138,    67,    66,
     117,    74,    66,   124,   125,   126,   127,    66,   248,    66,
     178,     1,     1,     3,     4,    72,    69,   138,     4,   181,
       3,     4,    71,    72,     1,   190,   141,    65,     1,    13,
      14,     1,   254,    71,    63,   178,    74,     1,    72,   178,
      66,   206,   179,   211,    68,     1,   190,     6,    72,   190,
      18,    64,    65,   271,   272,   273,    66,   178,   179,   196,
     181,    74,   227,   285,   229,    68,   288,    65,   211,   234,
     235,   236,   211,   238,   406,    65,    70,    66,    70,    65,
      70,    71,    65,   227,    74,    65,   227,    70,   229,    66,
     211,    74,     3,    66,     1,    72,    66,   337,    82,    83,
       1,    17,    66,   224,     3,     4,   227,   224,   229,    71,
      66,    79,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   349,   300,    65,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   301,    32,    33,    34,   300,    71,    66,
      65,   300,    80,   331,    42,    66,    65,    45,     3,     4,
      44,    45,    74,   413,   414,   415,     3,     4,   343,   300,
     402,    66,    84,    85,   406,    71,    72,    72,   331,   357,
      66,     3,   331,     5,     6,     7,    72,     9,    10,    66,
      74,    66,    29,   421,    81,    72,   349,    72,    69,   449,
     331,    66,    66,    73,   357,    66,    66,    72,    72,   459,
      66,    72,   462,   463,    67,   465,    72,    66,   349,    64,
      65,     3,   472,    66,   474,   475,   357,    66,    65,     3,
     480,    71,   120,   411,   263,   264,   265,   266,    15,    16,
      68,   416,   417,    65,   419,    70,    69,   425,    71,    72,
     138,    73,    74,    75,    76,    77,    78,    68,   411,    11,
      12,     1,   411,     3,     4,     5,     6,     7,   421,     9,
      10,    56,   425,   124,   125,   126,   451,   452,    68,   410,
     411,    75,    76,   410,     9,    10,    64,    65,    64,    65,
     421,   179,   467,    54,   425,    67,    71,    72,    71,    72,
     188,    68,   190,    71,    72,    71,    72,    71,   196,   261,
     262,    67,     1,    66,     3,     4,     5,     6,     7,    66,
       9,    10,   267,   268,    66,    65,    71,    67,   269,   270,
      66,    68,    70,    73,    74,    75,    76,    77,    78,   227,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    65,    51,    52,    53,    71,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    66,    65,    71,    68,    68,
      67,    66,    71,    66,    73,    74,    75,    76,    77,    78,
       1,    67,     3,     4,     5,     6,     7,    66,     9,    10,
      66,    68,    67,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   301,    66,     1,    67,    66,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,   119,
      51,    52,    53,    66,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    69,    65,   117,   410,    68,   105,   214,
      71,   188,    73,    74,    75,    76,    77,    78,     1,   141,
       3,     4,     5,     6,     7,   256,     9,    10,   164,   258,
     260,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,   257,   255,     1,   259,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    66,    65,    -1,    69,    68,    -1,    -1,    71,    -1,
      73,    74,    75,    76,    77,    78,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    -1,    -1,    68,    -1,    -1,    71,    -1,    73,    74,
      75,    76,    77,    78,     1,    -1,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,    -1,
       9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    65,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
       3,     4,     5,     6,     7,    -1,     9,    10,    63,    -1,
      65,    -1,    -1,    68,    -1,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    78,    -1,    -1,    29,    -1,    31,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,
       3,    -1,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    29,    -1,    31,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     3,     4,     5,
       6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    74,    75,
      76,    77,    78,     3,     4,     5,     6,     7,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,     3,     4,     5,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    78,     3,
       4,     5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    74,    75,    76,    77,    78,    65,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    74,    75,    76,
      77,    78,     3,     4,     5,     6,     7,    -1,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    65,     9,    10,    -1,    -1,    70,    -1,    -1,    73,
      74,    75,    76,    77,    78,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,    74,    75,    76,    77,    78,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,     3,     4,     5,     6,     7,    65,     9,    10,
      -1,    -1,    -1,    71,    -1,    73,    74,    75,    76,    77,
      78,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    78,     3,     4,
       5,     6,     7,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    -1,     1,    -1,    -1,    -1,     3,
      65,     5,     6,     7,    -1,     9,    10,    -1,    73,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,     0,    65,    -1,     3,
       4,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    65,    68,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     1,    -1,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    29,    -1,    31,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    29,    -1,    31,    -1,     1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    31,    -1,    -1,    66,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    31,    -1,    -1,    63,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    88,     0,     3,     4,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    65,    74,    89,    90,
      93,    95,    96,    97,    98,    99,   100,   101,   106,   109,
     113,   114,   115,   116,   160,    65,     3,    63,   113,   160,
      98,   116,   117,     1,    71,   103,   104,   113,   106,   106,
     106,    63,     3,    29,    65,   160,    63,    92,    93,    94,
      95,   131,    64,    65,     1,     3,    66,   106,   118,   119,
     120,   121,   122,   125,   114,     3,     5,     6,     7,     9,
      10,    65,    73,    74,    75,    76,    77,    78,   106,   125,
     152,   154,   155,   157,   159,    63,     3,   110,   111,   112,
       1,    66,    66,    98,   116,    71,    71,    72,    69,    91,
     102,   160,     1,     3,     7,     9,    10,    51,    52,    53,
      55,    56,    57,    58,    59,    60,    61,    62,    65,    68,
      71,    94,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   139,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   156,   160,    93,   131,
     113,     1,    67,   139,   140,   152,   160,    66,    64,    65,
     113,   116,   126,   127,     1,    66,     1,    66,    72,    72,
      65,   152,   152,   152,   131,   136,    65,   116,     1,    66,
       1,    66,   151,     8,     9,    10,    64,    65,    86,     6,
     110,    69,     1,    68,    72,    66,   104,    63,   123,   137,
      92,     1,    68,   105,   106,    68,    70,    65,   153,    65,
     153,   153,   140,    70,    65,    65,    65,   128,    65,     3,
      71,    71,    71,   136,   125,   160,     1,    68,   132,     1,
      68,   128,     1,    71,    72,    18,    79,    17,    80,    81,
      73,    15,    16,    13,    14,    82,    83,    11,    12,    75,
      76,    74,    84,    85,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    69,   138,     9,    10,   138,    67,
       1,    67,     1,    67,   140,     1,    66,   118,   126,   127,
      64,    65,    66,    66,    50,   121,     3,    50,   125,     1,
      66,     1,    66,    66,    66,     3,   136,     1,    66,   137,
     158,     3,     1,    68,   140,    68,   112,   123,   124,    68,
       1,    70,    71,   107,   108,   113,   128,    70,   128,   136,
     136,   136,    56,    71,   136,    71,     1,    71,     1,    66,
      68,     1,    68,    68,    71,   137,   142,    70,   141,   143,
     144,   145,   146,   147,   147,   148,   148,   148,   148,   149,
     149,   150,   150,   151,   151,   151,   137,   137,    67,    67,
       1,    67,    66,     1,    66,     1,    66,     1,    67,   140,
       1,    66,   118,     1,    66,    66,    66,     1,    67,    66,
       1,    66,    72,    68,     1,    68,    72,    71,   140,    71,
      72,    70,   128,    66,    66,    66,    65,    71,   136,    71,
      71,    66,   151,    68,   139,    70,    67,    66,    66,    67,
       1,    67,    66,     1,    66,    66,    67,    66,   137,    68,
      72,    68,   123,   108,   140,   128,   128,   128,   136,    66,
     136,    71,    71,   136,   151,   139,    67,    66,    68,    54,
      66,   128,    66,    66,   136,    66,   136,    71,   128,    71,
     128,   128,    66,   128,    66,    66,   136,   128,   128,   128,
      66,   128
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(c_char = YYEMPTY)
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
  if (c_char == YYEMPTY && c_len == 1)				\
    {								\
      c_char = (Token);						\
      c_lval = (Value);						\
      yytoken = YYTRANSLATE (c_char);				\
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


/* YYLEX -- calling `c_lex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX c_lex (YYLEX_PARAM)
#else
# define YYLEX c_lex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (c_debug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (c_debug)								  \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, c_STYPE const * const c_valuep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, c_valuep)
    FILE *yyoutput;
    int yytype;
    c_STYPE const * const c_valuep;
#endif
{
  if (!c_valuep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *c_valuep);
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
yy_symbol_print (FILE *yyoutput, int yytype, c_STYPE const * const c_valuep)
#else
static void
yy_symbol_print (yyoutput, yytype, c_valuep)
    FILE *yyoutput;
    int yytype;
    c_STYPE const * const c_valuep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, c_valuep);
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
  if (c_debug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (c_STYPE *c_vsp, int c_rule)
#else
static void
yy_reduce_print (c_vsp, c_rule)
    c_STYPE *c_vsp;
    int c_rule;
#endif
{
  int yynrhs = yyr2[c_rule];
  int yyi;
  unsigned long int yylno = yyrline[c_rule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     c_rule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[c_rule] + yyi],
		       &(c_vsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (c_debug)				\
    yy_reduce_print (c_vsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int c_debug;
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
  YYSIZE_T c_len;
  for (c_len = 0; yystr[c_len]; c_len++)
    continue;
  return c_len;
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
yysyntax_error (char *yyresult, int yystate, int c_char)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (c_char);
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

      /* Stay within bounds of both c_check and yytname.  */
      int c_checklim = YYLAST - yyn + 1;
      int yyxend = c_checklim < YYNTOKENS ? c_checklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (c_check[yyx + yyn] == yyx && yyx != YYTERROR)
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
yydestruct (const char *yymsg, int yytype, c_STYPE *c_valuep)
#else
static void
yydestruct (yymsg, yytype, c_valuep)
    const char *yymsg;
    int yytype;
    c_STYPE *c_valuep;
#endif
{
  YYUSE (c_valuep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, c_valuep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int c_parse (void *YYPARSE_PARAM);
#else
int c_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int c_parse (void);
#else
int c_parse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int c_char;

/* The semantic value of the lookahead symbol.  */
c_STYPE c_lval;

/* Number of syntax errors so far.  */
int c_nerrs;



/*-------------------------.
| c_parse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
c_parse (void *YYPARSE_PARAM)
#else
int
c_parse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
c_parse (void)
#else
int
c_parse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `c_ss': related to states.
       `c_vs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 c_ssa[YYINITDEPTH];
    yytype_int16 *c_ss;
    yytype_int16 *c_ssp;

    /* The semantic value stack.  */
    c_STYPE c_vsa[YYINITDEPTH];
    c_STYPE *c_vs;
    c_STYPE *c_vsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  c_STYPE c_val;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (c_vsp -= (N), c_ssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int c_len = 0;

  yytoken = 0;
  c_ss = c_ssa;
  c_vs = c_vsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  c_nerrs = 0;
  c_char = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  c_ssp = c_ss;
  c_vsp = c_vs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  c_ssp++;

 yysetstate:
  *c_ssp = yystate;

  if (c_ss + yystacksize - 1 <= c_ssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = c_ssp - c_ss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	c_STYPE *c_vs1 = c_vs;
	yytype_int16 *c_ss1 = c_ss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &c_ss1, yysize * sizeof (*c_ssp),
		    &c_vs1, yysize * sizeof (*c_vsp),
		    &yystacksize);

	c_ss = c_ss1;
	c_vs = c_vs1;
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
	yytype_int16 *c_ss1 = c_ss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (c_ss_alloc, c_ss);
	YYSTACK_RELOCATE (c_vs_alloc, c_vs);
#  undef YYSTACK_RELOCATE
	if (c_ss1 != c_ssa)
	  YYSTACK_FREE (c_ss1);
      }
# endif
#endif /* no yyoverflow */

      c_ssp = c_ss + yysize - 1;
      c_vsp = c_vs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (c_ss + yystacksize - 1 <= c_ssp)
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
  if (c_char == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      c_char = YYLEX;
    }

  if (c_char <= YYEOF)
    {
      c_char = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (c_char);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &c_lval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || c_check[yyn] != yytoken)
    goto yydefault;
  yyn = c_table[yyn];
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
  YY_SYMBOL_PRINT ("Shifting", yytoken, &c_lval, &yylloc);

  /* Discard the shifted token.  */
  c_char = YYEMPTY;

  yystate = yyn;
  *++c_vsp = c_lval;

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
  c_len = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  c_val = c_vsp[1-c_len];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 128 "../src/gram.y"
    {
	    (c_val.tok).start = 0;
	    (c_val.tok).end = 0;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 133 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start ? (c_vsp[(1) - (2)].tok).start : (c_vsp[(2) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 141 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 146 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 154 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    delete_sym(&(c_vsp[(1) - (2)].tok));
	    (c_val.tok).symbol = NULL;
#ifndef LMAIN
	    clearParaList(0);			/* restore all overloaded parameters */
#endif
	}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 163 "../src/gram.y"
    {
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as function name */
#endif
	}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 168 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    delete_sym(&(c_vsp[(2) - (4)].tok));			/* not deleted if imm */
#ifndef LMAIN
	    clearParaList(0);			/* restore all overloaded parameters */
#endif
	}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 179 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 184 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 192 "../src/gram.y"
    {	/* may have empty init_declarator_list according to K&R */
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;	/* GCC issues a warning: */
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;		/* "useless keyword or typname in empty declaration" */
	    (c_val.tok).symbol = NULL;		/* (see also rule 17) */
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as local var */
#endif
	}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 200 "../src/gram.y"
    {
	    Symbol *	sp;
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    sp = (c_vsp[(2) - (3)].tok).symbol;
	    assert(sp);			/* ERROR: initialized in : init_declarator */
	    while (sp) {
		Symbol * sp1 = sp;
		sp = sp->next;		/* get next before Symbol is placed or deleted */
		if ((c_vsp[(1) - (3)].tok).symbol && (c_vsp[(1) - (3)].tok).symbol->type == UDF) {
		    sp1->type = CTYPE;		/* found a typedef */
#if YYDEBUG
		    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\nP %-15s %d %d\n", sp1->name, sp1->type, sp1->ftype);
#endif
		    if (lookup(sp1->name) == 0) {
			link_sym(sp1);
		    }
		} else {
		    (c_val.tok).symbol = sp1;		/* use $$ as transport Token for delete_sym */
		    delete_sym(&(c_val.tok));
		}
	    }
	    (c_val.tok).symbol = NULL;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as local var */
#endif
	}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 227 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as local var */
#endif
	    yyclearin; yyerrok;
	}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 239 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 244 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 252 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
#ifndef LMAIN
	    if (lexflag & C_FUNCTION) {
		lexflag |= C_PARA;		/* function internal C var is coming */
	    }
#endif
	}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 262 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (2)].tok).symbol;		/* typedef information */
#ifndef LMAIN
	    if (lexflag & C_FUNCTION) {
		lexflag |= C_PARA;		/* function internal C var is coming */
	    }
#endif
	}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 275 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 280 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 285 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 290 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = &typedefSymbol;
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 298 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 303 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 308 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 313 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 318 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 323 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 328 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 333 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 338 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 343 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 348 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 353 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 358 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 363 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 368 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 374 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 379 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 388 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 393 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 401 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 406 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 411 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 420 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 425 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    delete_sym(&(c_vsp[(2) - (2)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 432 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 438 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 446 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 451 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 459 "../src/gram.y"
    {		/* optional in modern C compilers (not K&R) */
	    (c_val.tok).start = 0;
	    (c_val.tok).end = 0;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 464 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start ? (c_vsp[(1) - (2)].tok).start : (c_vsp[(2) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 472 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 477 "../src/gram.y"
    {
	    Symbol *	sp;
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    sp = (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
	    assert(sp);			/* ERROR: initialized in : init_declarator */
	    if (sp->type == CWORD) {
		while (sp->next) {
		    sp = sp->next;
		}
		sp->next = (c_vsp[(3) - (3)].tok).symbol;	/* place in a list */
	    }
	}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 493 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 498 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
	}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 506 "../src/gram.y"
    {/* nameless struct/union for moder C (not K&R) */
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;	/* should only be for struct or union, but GCC */
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;		/* only issues warning: "declaration does not declare anything" */
	    (c_val.tok).symbol = NULL;		/* for 'TYPE ;' in a struct_declaration (similar to 'declaration' rule 5) */
	}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 511 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 516 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 525 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 530 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 535 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 540 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 548 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 553 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 561 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    delete_sym(&(c_vsp[(1) - (1)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 567 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 572 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(1) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 581 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 586 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 592 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    delete_sym(&(c_vsp[(2) - (5)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 598 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (6)].tok).start;
	    (c_val.tok).end = (c_vsp[(6) - (6)].tok).end;
	    delete_sym(&(c_vsp[(2) - (6)].tok));
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 605 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    delete_sym(&(c_vsp[(2) - (2)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 614 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 619 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 627 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 632 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 640 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    delete_sym(&(c_vsp[(1) - (1)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 646 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(1) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 655 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
#ifndef LMAIN
	    if ((c_vsp[(1) - (1)].tok).symbol->type < MAX_LS &&
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(1) - (1)].tok).symbol->v_glist == 0
#else
		(c_vsp[(1) - (1)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immVarRemove((c_vsp[(1) - (1)].tok).start, (c_vsp[(1) - (1)].tok).end, (c_vsp[(1) - (1)].tok).symbol);
	    }
#endif
	}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 671 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(2) - (2)].tok).symbol;
#ifndef LMAIN
	    if ((c_vsp[(2) - (2)].tok).symbol->type < MAX_LS &&
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(2) - (2)].tok).symbol->v_glist == 0
#else
		(c_vsp[(2) - (2)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immVarRemove((c_vsp[(2) - (2)].tok).start, (c_vsp[(2) - (2)].tok).end, (c_vsp[(2) - (2)].tok).symbol);
	    }
#endif
	}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 690 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 695 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 700 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(2) - (3)].tok).symbol;
	}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 705 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(2) - (4)].tok).symbol;
	    yyclearin; yyerrok;
	}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 711 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
	}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 716 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (4)].tok).symbol;
	    yyclearin; yyerrok;
	}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 722 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (4)].tok).symbol;
	}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 727 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (5)].tok).symbol;
	    yyclearin; yyerrok;
	}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 733 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (2)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 741 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	    yyclearin; yyerrok;
	}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 750 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 758 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (4)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	    yyclearin; yyerrok;
	}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 767 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (3)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 775 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (4)].tok).symbol;
#ifndef LMAIN
	    lexflag &= ~C_PARA;			/* end of overloading imm as parameter */
#endif
	    yyclearin; yyerrok;
	}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 787 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (2)].tok).symbol;
#ifndef LMAIN
	    clearParaList(1);			/* imm vars are temporarily overloaded */
#endif
	}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 798 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 803 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 808 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 813 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 821 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 826 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 834 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 839 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 847 "../src/gram.y"
    {			/* K&R C parameter list without types */
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 852 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 860 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 865 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 873 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    delete_sym(&(c_vsp[(2) - (2)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 879 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 887 "../src/gram.y"
    {				/* K&R C parameter list without types */
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    delete_sym(&(c_vsp[(1) - (1)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 893 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(3) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 902 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 907 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 912 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 918 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 923 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 932 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 937 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 945 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 950 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 958 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 963 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 968 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 976 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 981 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 987 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 992 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 998 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1003 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1009 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1014 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1020 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1025 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1031 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1036 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1042 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1047 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1053 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1058 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1064 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1069 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1078 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1083 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1088 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1093 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1098 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1103 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1111 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(1) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1117 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1122 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1130 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1135 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1140 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1149 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1154 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1160 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1165 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1171 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1176 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1182 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1187 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1196 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1201 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1209 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1214 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (7)].tok).start;
	    (c_val.tok).end = (c_vsp[(7) - (7)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1219 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1227 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1232 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (7)].tok).start;
	    (c_val.tok).end = (c_vsp[(7) - (7)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1237 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (6)].tok).start;
	    (c_val.tok).end = (c_vsp[(6) - (6)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1242 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (7)].tok).start;
	    (c_val.tok).end = (c_vsp[(7) - (7)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1247 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (7)].tok).start;
	    (c_val.tok).end = (c_vsp[(7) - (7)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1252 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (8)].tok).start;
	    (c_val.tok).end = (c_vsp[(8) - (8)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1257 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (7)].tok).start;
	    (c_val.tok).end = (c_vsp[(7) - (7)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1262 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (8)].tok).start;
	    (c_val.tok).end = (c_vsp[(8) - (8)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1267 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (8)].tok).start;
	    (c_val.tok).end = (c_vsp[(8) - (8)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1272 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (9)].tok).start;
	    (c_val.tok).end = (c_vsp[(9) - (9)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1280 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(2) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1286 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1291 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1296 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1301 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1306 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1315 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1320 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1328 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1334 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1340 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\n<%u = %u>\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(1) - (3)].tok).symbol->v_glist == 0
#else
		(c_vsp[(1) - (3)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immAssignFound((c_vsp[(1) - (3)].tok).start, (c_vsp[(2) - (3)].tok).start, (c_vsp[(3) - (3)].tok).end, (c_vsp[(1) - (3)].tok).symbol, 6);
	    }
#endif
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1362 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1367 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1372 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1377 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1382 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1387 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1392 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1397 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1402 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1407 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1412 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1420 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1425 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1431 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
#ifndef LMAIN
	    if (iC_Pflag) {
		Symbol * sp = (c_vsp[(4) - (4)].tok).symbol;
		warning("ISO C forbids omitting the middle term of a ?: expression", sp ? sp->name : NULL);
	    }
	    (c_val.tok).symbol = NULL;
#endif
	}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1445 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1453 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1458 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1466 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1471 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1479 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1484 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1492 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1497 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1505 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1510 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1518 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1523 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1528 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1536 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1541 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1546 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1551 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1556 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1564 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1569 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1574 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1582 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1587 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1592 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1600 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1605 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1610 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1615 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1623 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1628 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1633 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1638 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1647 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1652 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1657 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1662 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1667 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1672 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1677 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1686 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1691 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\n<++%u %u>\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(2) - (2)].tok).symbol->v_glist == 0
#else
		(c_vsp[(2) - (2)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immAssignFound((c_vsp[(2) - (2)].tok).start, (c_vsp[(1) - (2)].tok).start, (c_vsp[(2) - (2)].tok).end, (c_vsp[(2) - (2)].tok).symbol, 0);
	    }
#endif
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1710 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\n<--%u %u>\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(2) - (2)].tok).symbol->v_glist == 0
#else
		(c_vsp[(2) - (2)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immAssignFound((c_vsp[(2) - (2)].tok).start, (c_vsp[(1) - (2)].tok).start, (c_vsp[(2) - (2)].tok).end, (c_vsp[(2) - (2)].tok).symbol, 1);
	    }
#endif
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1729 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1737 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1742 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1747 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1752 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1757 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1762 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1770 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1775 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1780 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1786 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1791 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1797 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1802 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (5)].tok).start;
	    (c_val.tok).end = (c_vsp[(5) - (5)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1808 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(3) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1814 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    delete_sym(&(c_vsp[(3) - (3)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1820 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1825 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1833 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
	}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1838 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\n<%u %u++>\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(1) - (2)].tok).symbol->v_glist == 0
#else
		(c_vsp[(1) - (2)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immAssignFound((c_vsp[(1) - (2)].tok).start, (c_vsp[(2) - (2)].tok).start, (c_vsp[(2) - (2)].tok).end, (c_vsp[(1) - (2)].tok).symbol, 2);
	    }
#endif
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1857 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "\n<%u %u-->\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_vsp[(1) - (2)].tok).symbol->v_glist == 0
#else
		(c_vsp[(1) - (2)].tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immAssignFound((c_vsp[(1) - (2)].tok).start, (c_vsp[(2) - (2)].tok).start, (c_vsp[(2) - (2)].tok).end, (c_vsp[(1) - (2)].tok).symbol, 3);
	    }
#endif
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1879 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    delete_sym(&(c_vsp[(1) - (1)].tok));
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1885 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1890 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1895 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1900 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1906 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1911 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (4)].tok).start;
	    (c_val.tok).end = (c_vsp[(4) - (4)].tok).end;
	    (c_val.tok).symbol = NULL;
	    yyclearin; yyerrok;
	}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1920 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1925 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1933 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1938 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (2)].tok).start;
	    (c_val.tok).end = (c_vsp[(2) - (2)].tok).end;
	    (c_val.tok).symbol = NULL;
	}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1946 "../src/gram.y"
    {
	    (c_val.tok).start = (c_vsp[(1) - (1)].tok).start;
	    (c_val.tok).end = (c_vsp[(1) - (1)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(1) - (1)].tok).symbol;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "[%u %u]\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_val.tok).symbol->v_glist == 0
#else
		(c_val.tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immVarFound((c_val.tok).start, (c_val.tok).end, (c_vsp[(1) - (1)].tok).symbol);
	    }
#endif
	}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1965 "../src/gram.y"
    {
	    /* stops this being a primary_expression which would lead to C assignment */
	    (c_val.tok).start = (c_vsp[(1) - (3)].tok).start;
	    (c_val.tok).end = (c_vsp[(3) - (3)].tok).end;
	    (c_val.tok).symbol = (c_vsp[(2) - (3)].tok).symbol;
#ifndef LMAIN
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "{%u %u}\n", (c_val.tok).start, (c_val.tok).end);
#endif
	    if (
#if YYDEBUG && ! defined(SYUNION)
		(c_val.tok).symbol->v_glist == 0
#else
		(c_val.tok).symbol->v_cnt <= 2		/* v_cnt instead of v_glist for SYUNION */
#endif
	    ) {
		immVarFound((c_val.tok).start, (c_val.tok).end, NULL);	/* moves pStart and pEnd without changing vStart vEnd */
	    }
#endif
	}
    break;



/* Line 1455 of yacc.c  */
#line 5556 "gram.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &c_val, &yyloc);

  YYPOPSTACK (c_len);
  c_len = 0;
  YY_STACK_PRINT (c_ss, c_ssp);

  *++c_vsp = c_val;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *c_ssp;
  if (0 <= yystate && yystate <= YYLAST && c_check[yystate] == *c_ssp)
    yystate = c_table[yystate];
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
      ++c_nerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, c_char);
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
	    (void) yysyntax_error (yymsg, yystate, c_char);
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

      if (c_char <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (c_char == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &c_lval);
	  c_char = YYEMPTY;
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
  YYPOPSTACK (c_len);
  c_len = 0;
  YY_STACK_PRINT (c_ss, c_ssp);
  yystate = *c_ssp;
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
	  if (0 <= yyn && yyn <= YYLAST && c_check[yyn] == YYTERROR)
	    {
	      yyn = c_table[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (c_ssp == c_ss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], c_vsp);
      YYPOPSTACK (1);
      yystate = *c_ssp;
      YY_STACK_PRINT (c_ss, c_ssp);
    }

  *++c_vsp = c_lval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], c_vsp, yylsp);

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
  if (c_char != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &c_lval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (c_len);
  YY_STACK_PRINT (c_ss, c_ssp);
  while (c_ssp != c_ss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*c_ssp], c_vsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (c_ss != c_ssa)
    YYSTACK_FREE (c_ss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1992 "../src/gram.y"


#ifdef LMAIN
static void
yyerror(char *s, ...)
{
    fprintf(iC_outFP, "\n%*s\n%*s\n", column, "^", column, s);
    fflush(iC_outFP);
} /* yyerror */

void
ierror(						/* print error message */
    char *	str1,
    char *	str2)
{
    fprintf(iC_outFP, "*** Error: %s", str1);
    if (str2) {
	fprintf(iC_outFP, " %s", str2);
    } else {
	putc('.', iC_outFP);
    }
    fprintf(iC_outFP, "\n");
    fflush(iC_outFP);
} /* ierror */
#else

static char*	macro[] = { MACRO_NAMES };

/********************************************************************
 *
 *	immVarFound
 *
 *	The parser has found a bare immediate variable or an immediate
 *	variable in parentheses. (zany but allowed as an lvalue in C)
 *	To locate parentheses, only pStart and pEnd are adjusted.
 *	vStart and vEnd still mark the original variable.
 *
 *	sp == 0 is used to signal parenthesized immediate variable.
 *
 *******************************************************************/

static void
immVarFound(unsigned int start, unsigned int end, Symbol* sp)
{
    LineEntry *	newArray;

    if (sp) {
	lep->vStart = start;			/* of an imm variable */
	lep->equOp  = LARGE;			/* marks a value variable */
	lep->vEnd   = end;			/* of an imm variable */
	lep->sp     = sp;
	lep->ppIdx  = 6;
	if (sp->ftype != ARITH && sp->ftype != GATE && sp->type != ERR) {
	    ierror("C-statement tries to access an imm type not bit or int:", sp->name);
	    if (! iFunSymExt) sp->type = ERR;	/* cannot execute properly */
	}
    } else {					/* parenthesized variable found */
	--lep;					/* step back to previous entry */
    }
    lep->pStart = start;			/* of possible parentheses */
    lep->pEnd   = end;				/* of possible parentheses */
    lep++;
    if (lep > &lineEntryArray[udfCount-2]) {	/* allow for 2 guard entries at end */
	udfCount += LEAI;			/* increase the size of the array */
	newArray = (LineEntry*)realloc(lineEntryArray, udfCount * sizeof(LineEntry));
	assert(newArray);			/* FIX with quit */
	lep += newArray - lineEntryArray;
	lineEntryArray = newArray;		/* Array has been successfully resized */
    }
} /* immVarFound */

/********************************************************************
 *
 *	immVarRemove
 *
 *	The parser has found a bare immediate variable which is being
 *	declared as a C variable in C code. Remove it from the
 *	lineEntryArray and mark it so it is returned as a C-IDENTIFIER
 *	either globally or for the duration of this function. This
 *	effectively hides the imm Symbol with the same name in C code.
 *
 *******************************************************************/

static void
immVarRemove(unsigned int start, unsigned int end, Symbol* sp)
{
    --lep;					/* step back to previous entry */
    if(lep->pStart == start && lep->pEnd == end) {
	markParaList(sp);			/* mark imm Symbol so it is hidden */
	lep->pStart = lep->equOp = LARGE;
    } else {
#if YYDEBUG
	if ((iC_debug & 0402) == 0402)
	    fprintf(iC_outFP, "stack position (remove): start %u (%u) end %u (%u) Symbol %s\n",
	    lep->pStart, start, lep->pEnd, end, lep->sp->name);
#endif
	ierror("C name which is an imm variable cannot be hidden:", sp->name);
	lep++;					/* do not remove */
    }
} /* immVarRemove */

/********************************************************************
 *
 *	immAssignFound
 *
 *	The parser has found an assignment to an immediate variable
 *	The assignment operator may be either
 *	= which is simple assignment or
 *	+= -= *= /= %= &= ^= |= >>= <<= which is an operator assignment or
 *	++ -- with ppi 0 or 1, which is pre-increment/decrement or
 *	++ -- with ppi 2 or 3, which is post-increment/decrement.
 *
 *	Backtrack along the immediate variables found so far, to find
 *	the one being assigned to here. Its 'lep' entry is modified.
 *	equOp points to an actual assignment or locates inc/dec-operator.
 *	vStart and vEnd still mark the variable being assigned to.
 *	pEnd is adjusted to the very end of the expression being assigned,
 *	where the closing bracket for the assignment macro hast to be placed.
 *
 *	If no suitable immediate variable is found it is a compiler
 *	error, because an immediate assignment should have a bare
 *	immediate variable as its first token (same start position)
 *	Alternatively an immediate variable in parentheses is accepted.
 *
 *	Checked earlier that IMM_IDENTIFIER is ftype ARITH or GATE.
 *
 *	ftyp is derived from the ftype of the Symbol. It may only be
 *	ARITH === 1, GATE === 2, ARITH+2 and GATE+2. (UDFA === 0 is an error)
 *
 *	This did not work. jw 2004.02.23 found with Electric Fence 2.2.1
 *	in arnc5.ic:
 *		imm clock clk; if (IX0.1) { clk = 1; } // tries to use
 *		macro[ftyp] with ftyp = CLCKL 18, type = ERR 19 // FIXED
 *
 *	Use of ppIdx, ppi and equOp, operator:
 *	In immVarFound() p->ppIdx is initialized to 6 and p->equOp to LARGE
 *	which is then described as a used variable. If equOp is set to some
 *	operator position it becomes an assigned operator. All other imm
 *	variables in an assignment, which have ppIdx == 6 have the value
 *	set to 5. This marks the variable as used, even if it is an assignment
 *	variable. By this method multiple assignments are handled correctly.
 *	If ppi < 5 the current variable is involved in a pre/post-inc/dec
 *	operation and is marked as both assigned and used anyway.
 *
 *******************************************************************/

static void
immAssignFound(unsigned int start, unsigned int operator, unsigned int end, Symbol* sp, int ppi)
{
    LineEntry *	p;
    int		ftyp;
    int		typ;

    assert(sp);
#if YYDEBUG
	if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "start %u, op %u end %u sp =>%s ppi %d\n", start, operator, end, sp->name, ppi);
#endif

    for (p = lep - 1; p >= lineEntryArray; p-- ) {
#if YYDEBUG
	if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "--%u(%u %u %u)%u %d\n", p->pStart, p->vStart, p->equOp==LARGE?0:p->equOp, p->vEnd, p->pEnd, p->ppIdx);
#endif
	if (p->pStart == start) {		/* start position of imm variable assigned to */
	    p->equOp  = operator;		/* position of operator marks an assignment expression */
	    p->pEnd   = end;			/* end position of expression assigned from */
	    p->ppIdx  = ppi;			/* pre/post-inc/dec character value */
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "= %u(%u %u %u)%u %d\n", p->pStart, p->vStart, p->equOp==LARGE?0:p->equOp, p->vEnd, p->pEnd, p->ppIdx);
#endif
	    if (p->sp == sp) {
		typ = sp->type;
		ftyp = sp->ftype;
		if (typ == UDF) {
		    if (iC_Sflag) {
			ierror("strict: C assignment to an imm variable (should be immC):", sp->name);
			if (! iFunSymExt) sp->type = ERR;	/* cannot execute properly */
			else goto changeType;
		    } else {
			changeType:
			sp->type = iC_ctypes[ftyp];	/* must be ARNC or LOGC */
			listGenOut(sp);		/* list immC node and generate possible output */
		    }
		} else if ((typ != ARNC || ftyp != ARITH) &&
			   (typ != LOGC || ftyp != GATE) &&
			   (typ != ERR)) {	/* avoids multiple error messages */
		    if (((typ == ARN || typ == ARNF) && ftyp == ARITH) ||
		        (typ >= MIN_GT && typ < MAX_GT && ftyp == GATE)) {
			ierror("C-assignment to an imm variable already assigned in iC code:", sp->name);
		    } else {
			ierror("C-assignment to an incompatible imm type:", sp->name);
		    }
		    if (! iFunSymExt) sp->type = ERR;	/* cannot execute properly */
		}
		return;
	    } else {
		break;				/* Error: Symbols don't match */
	    }
	} else if (p->pStart < start) {
	    break;				/* Error: will not find start */
	}
	if (p->ppIdx == 6) {			/* all other imm values in the assignment */
	    p->ppIdx = 5;			/* are used even if themselves assigned to */
	}
    }
    execerror("C-assignment: Symbol not found ???", sp->name, __FILE__, __LINE__);
} /* immAssignFound */

/********************************************************************
 *
 *	pushEndStack
 *
 *******************************************************************/

static unsigned int
pushEndStack(unsigned int value)
{
    assert(esp < &endStack[ENDSTACKSIZE]);
    return *esp++ = value;
} /* pushEndStack */

/********************************************************************
 *
 *	popEndStack
 *
 *******************************************************************/

static unsigned int
popEndStack(void)
{
    assert(esp >= endStack);
    return *--esp;
} /* popEndStack */

/********************************************************************
 *
 *	copyAdjustFile
 *
 *	Allocation of the lineEntryArray[] is carried out if iFP == 0.
 *
 *	There is one entry for every occurence of an immediate variable
 *	in the C code. Since only immediate variables which are still
 *	unassigned by the time the C-parse is executed are proper
 *	candidates and if 10 times this number is allowed for, the array
 *	is unlikely to overflow. This count is available by counting
 *	Symbols of type == UDF. This is also done later in listNet(),
 *	but we need the value earlier.
 *
 *	Also other imm variables are used as values, so a generous
 *	first guess and realloc() in immVarFound is a better choice.
 *
 *	Use the byte postions stored in lineEntryArray to insert
 *	macro calls for immediate variables and assignments
 *
 *	'lp' points to the ffexpr head when called as an auxiliary
 *	compile of one if - else or switch C fragment. Else NULL
 *
 *******************************************************************/

void
copyAdjust(FILE* iFP, FILE* oFP, List_e* lp)
{
    int			c;
    LineEntry*		p;
    unsigned int	start;
    unsigned int	vstart;
    unsigned int	vend;
    unsigned int	equop;
    unsigned int	earlyop;
    unsigned int	endop;
    unsigned int	end;
    unsigned int	bytePos;
    int			pFlag;
    int			ppi;
    Symbol *		sp;
    Symbol *		tsp;
    List_e *		lp1;
    List_e *		lp2;
    int			lNr = 0;
    int			sNr = 0;
#ifdef LEAS
    Symbol **		hsp;
#endif
    int			ml;
    int			ftyp;
    Symbol *		fsp = 0;
    static char *	f = "_f0_1";		/* name of literal function head */

    if (iFP == NULL) {
#ifdef LEAS
	for (hsp = symlist; hsp < &symlist[HASHSIZ]; hsp++) {
	    for (sp = *hsp; sp; sp = sp->next) {
		if (sp->type == UDF) {
		    udfCount += LEAS;
		}
	    }
	}
#endif
	lineEntryArray = (LineEntry*)realloc(NULL, udfCount * sizeof(LineEntry));
	assert(lineEntryArray);			/* FIX with quit */
	lep = lineEntryArray;
	lep->pStart = lep->equOp = LARGE;	/* guard value in case first immVarFound(0) */
	lep++;
	lep->pStart = lep->equOp = LARGE;	/* value overwritten by first immVarFound */
	return;					/* lineEntryArray initialized */
    }

    if (lp) {					/* ffexpr head link */
	assert(lp->le_val>>8 && lp->le_next == 0); /* must have function # and callled only once */
	tsp = lp->le_sym;			/* master - must be ftype F_CF, F_CE or F_SW */
	assert(tsp && (tsp->ftype == F_CF || tsp->ftype == F_CE || tsp->ftype == F_SW));
	lp1 = tsp->u_blist;
	assert(lp1);
	tsp = lp1->le_sym;			/* clock for ffexpr head */
	assert(tsp);
	if (tsp->ftype == CLCKL) {
	    lNr = 2;				/* start of value list for C fragment */
	} else if (tsp->ftype == TIMRL) {
	    lNr = 3;				/* extra space for timer delay */
	} else {
	    assert(0);				/* must be a clock */
	}
    }

    lep->pStart   = lep->equOp = LARGE;		/* finalise lineEntryArray */
    lep++;
    lep->pStart   = lep->equOp = LARGE;		/* require 2 guard entries at the end */

    bytePos = 0;
    p       = lineEntryArray + 1;		/* miss guard entry at start */
    start   = p->pStart;
    if (p->equOp < start) {
	earlyop = p->equOp;			/* operator in pre-inc/dec entry handled early */
#if YYDEBUG
	if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "load bytePos = %u, earlyop = %u\n", bytePos, earlyop);
#endif
    } else {
	earlyop = LARGE;
    }
    vstart  = LARGE;
    vend    = LARGE;
    equop   = LARGE;
    endop   = LARGE;
    end     = LARGE;
    pFlag   = 1;				/* start by outputting C code till first variable or ++/-- */
    sp      = NULL;

    while ((c = getc(iFP)) != EOF) {
	while (bytePos >= end) {
	    putc(')', oFP);			/* end found - output end */
	    end = popEndStack();
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "popE bytePos = %u, earlyop = %u, end = %u\n", bytePos, earlyop, end);
#endif
	}
	if (bytePos >= start) {
	    pushEndStack(end);			/* push previous end */
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "= %u(%u %u %u)%u %d\n", p->pStart, p->vStart, p->equOp==LARGE?0:p->equOp, p->vEnd, p->pEnd, p->ppIdx);
#endif
	    vstart = p->vStart;			/* start of actual variable */
	    vend   = p->vEnd;			/* end of actual variable */
	    equop  = p->equOp;			/* operator in this entry */
	    end    = p->pEnd;			/* end of this entry */
	    sp     = p->sp;			/* associated Symbol */
	    ppi    = p->ppIdx;			/* pre/post-inc/dec character value */
	    assert(sp);
	    ml     = lp		     ? 0		: MACRO_LITERAL;
	    ftyp  = sp->type == ERR ? UDFA
				     : (equop == LARGE) ? sp->ftype
							: sp->ftype + MACRO_OFFSET;
	    if (ppi >= 5) {
		/* assignment macro must be printed outside of enclosing parentheses */
		fprintf(oFP, macro[ml+ftyp]);	/* entry found - output macro start */
	    }
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "strt bytePos = %u, earlyop = %u, equop = %u, ppi = %d, sp =>%s\n", bytePos, earlyop, equop, ppi, sp->name);
#endif
	    p++;				/* next entry to locate possible earlyop */
	    assert(start <= vstart);
	    assert(vstart < vend);
	    assert(vend <= end);
	    assert(bytePos < p->pStart);
	    start = p->pStart;			/* start of next entry */
	    if (p->equOp < start) {
		earlyop = p->equOp;		/* operator in pre-inc/dec entry handled early */
#if YYDEBUG
		if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "load bytePos = %u, earlyop = %u\n", bytePos, earlyop);
#endif
	    } else {
		earlyop = LARGE;
	    }
	}
	if (bytePos == vstart) {
	    assert(sp);				/* start of actual variable */
	    if (lp) {
		functionUse[0].c_cnt |= F_FFEXPR;	/* flag for copying ffexpr macro */
		lp1 = lp;
	    } else {
		functionUse[0].c_cnt |= F_LITERAL;	/* flag for copying literal macro */
		if (fsp) {
		    lp1 = fsp->list;		/* start with head of pointer list */
		} else
		if ((fsp = lookup(f)) == 0) {
		    fsp = install(f, OR, F_CF);	/* install new literal function head */
		    lp1 = sy_push(fsp);		/* head of literal pointer list */
		    fsp->list = lp1;		/* save in list for more pointers */
		} else {
		    if (fsp->type != CF && fsp->ftype != F_CF) {
			ierror("use of literal function head which was previously used for a different purpose:", f);
		    }
		    lp1 = fsp->list;		/* start with head of pointer list */
		}
		lNr = 0;
	    }
	    sNr = lNr;				/* start at array pos after clock or timer,val */
	    while ((lp2 = lp1->le_next) != 0) {
		if (sp == lp2->le_sym) {
		    assert((lp2->le_val & VAR_MASK) == sNr);
		    break;			/* variable occurred previously */
		}
		sNr++;
		assert(sNr < VAR_MASK);		/* limits # of variables to 16.384 if short */
		lp1 = lp2;
	    }
	    if (lp2 == 0) {
		lp2 = sy_push(sp);		/* new variable in this C fragment */
		lp2->le_val |= sNr;		/* offset in list */
		lp1->le_next = lp2;		/* place at end of list */
	    }
	    if (equop == LARGE) {
		lp2->le_val |= VAR_USE;		/* variable value is only used */
	    } else {
		lp2->le_val |= VAR_ASSIGN;	/* variable is assigned to */
		if (ppi < 6) {
		    lp2->le_val |= VAR_USE;	/* pre/post-inc/dec or marked as used */
		}
	    }
	    if (ppi >= 5) {
		fprintf(oFP, "%d", sNr);	/* output Symbol pointer offset */
	    } else {
		/* expanded pre/post-inc/dec macro may be printed inside enclosing parentheses */
		if (ppi < 2){
		    /* pre-increment/decrement */
		    fprintf(oFP, "%s%d , %s%d) %c 1",
			macro[ml+ftyp], sNr, macro[ml+sp->ftype], sNr, idOp[ppi]);
		    /* ++x; produces: iC_AA(2 , iC_AV(2) + 1); */
		} else {
		    /* post-increment/decrement */
		    iC_Tflag = 1;		/* triggers definition of iC_tVar in C outfile */
		    fprintf(oFP, "(iC_tVar = %s%d), %s%d , iC_tVar %c 1), iC_tVar",
			macro[ml+sp->ftype], sNr, macro[ml+ftyp], sNr, idOp[ppi]);
		    /* x--; produces: (iC_tVar = iC_AV(2), iC_AA(2 , iC_tVar - 1), iC_tVar); */
		}
	    }
	    pFlag = 0;				/* start of variable name, which is replaced by macro */
	}
	if (bytePos == vend) {
	    pFlag = 1;				/* end of the variable which is the next entry */
	}
	if (bytePos == equop) {
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "use  bytePos = %u, earlyop = %u, equop = %u\n", bytePos, earlyop, equop);
#endif
	    if (ppi >= 5) {
		if (c == '=') {
		    c = ',';			/* simple assignment, replace '=' by ',' */
		} else {
		    assert(sp);
		    assert(strchr("+-*/%&^|><", c));
		    /********************************************************************
		     *  output variable as value for operator assignment expression
		     *  QB1 *= IB2 + IB3; produces:
		     *  iC_AA(2 , iC_AV(2) * ( iC_AV(3) + iC_AV(4)));
		     *  NOTE: parenthesis around operator assigned expression
		     *******************************************************************/
		    fprintf(oFP, ", %s%d) ", macro[ml+sp->ftype], sNr);
		    endop = equop + ((c == '>' || c == '<') ? 2 : 1);	/* replace '=' by " (" */
		    pushEndStack(end);		/* push extra end ')' for operator assign expression */
		    assert(lp2);
		    lp2->le_val |= VAR_USE;	/* operator assignment is also used */
		}
	    } else {
		assert(ppi >= 2);		/* equop occurred before start for pre-inc/dec */
		endop = bytePos + 1;		/* ppi >= 2 && < 5 is post-inc/dec at equop */
		pFlag = 0;			/* suppress output of ++ or -- from post-inc/dec */
	    }
	}
	if (bytePos == earlyop) {
#if YYDEBUG
	    if ((iC_debug & 0402) == 0402) fprintf(iC_outFP, "use  bytePos = %u, earlyop = %u\n", bytePos, earlyop);
#endif
	    endop = bytePos + 1;
	    pFlag = 0;				/* suppress output of ++ or -- from pre-inc/dec */
	}
	if (pFlag) {
	    if (bytePos != endop) {
		putc(c, oFP);			/* output all except variables and ++ or -- from inc/dec */
	    } else {
		fprintf(oFP, " (");		/* replace '=' by " (" for operator assignment */
	    }
	}
	if (bytePos == endop) {
	    endop = LARGE;
	    pFlag = 1;
	}
	bytePos++;
    }
    if ((iC_debug & 04) && (fsp = lookup(f)) != 0) {
	/********************************************************************
	 * compile listing output for literal function block
	 *******************************************************************/
	lp1 = fsp->list;			/* start with head of pointer list */
	assert(lp1->le_sym == fsp);
	fprintf(iC_outFP, "\n\n\t%s\t%c ---%c\t\t\t// (L)\n", fsp->name, iC_fos[fsp->ftype],
	    iC_os[CF]);
	while ((lp1 = lp1->le_next) != 0) {
	    Symbol *	gp;
	    int		use;
	    gp = lp1->le_sym;
	    assert(gp);
	    assert(gp->name);
	    use = lp1->le_val >> USE_OFFSET;
	    assert(use < Sizeof(iC_useText));
	    fprintf(iC_outFP, "\t%s\t%c<---%c\t\t\t// %d %s", gp->name, iC_fos[gp->ftype],
		iC_os[CF], lp1->le_val & 0xff, iC_useText[use]);
	    if ((iC_debug & 0200) && ! iFunSymExt) {	/* could use liveDisp, if it were global */
		fprintf(iC_outFP, "\t%s\t=", gp->name);
	    }
	    fprintf(iC_outFP, "\n");
	}
    }
} /* copyAdjust */
#endif

