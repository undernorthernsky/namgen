
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
     SRC_DIR = 296,
     PATCHES = 297
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
#define PATCHES 297




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


