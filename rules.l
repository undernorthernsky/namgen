%{
#include <stdio.h>

#define YYSTYPE char*
#include "y.tab.h"

extern int yyget_lineno();
extern void set_parsererror(int, int, char*);

static void lexer_error()
{
   set_parsererror(0, yyget_lineno(), yytext);
}

int found_verbatim = 0;

static void abort_parsing_now()
{
   found_verbatim = 1;
}

#define ECHO lexer_error()

%}

%option warn
%option yylineno
%option nounput
%option noinput
%%
program                 return PROGRAM; 
library                 return LIBRARY;
worker                  return WORKER;
src                     return SRC;
cmd                     return COMMAND;
depends                 return DEPENDS; 
flags                   return FLAGS;
libs                    return LIBS;
ld-flags                return LDFLAGS;
extra-obj               return ADD_OBJECTS;
export-inc              return EXPORT_INC;
dest-dir                return DESTDIR;
skip-install            return SKIP_INSTALL;
skip-shared             return SKIP_SHARED;
skip-static             return SKIP_STATIC;
convenience             return CONVENIENCE_LIB;
version-info            return VERSION_INFO;
version-number          return VERSION_NUMBER;
true                    return TRUE_VALUE;
false                   return FALSE_VALUE;
VERBATIM                abort_parsing_now(); return EOF; 
skip-if                 return SKIP_IF;

!                       return EXPR_MARK;
=                       return EQUALS;
\"                      return QUOTE;
\{                      return OBRACE;
\}                      return EBRACE;
\/\*                      return OCOMMENT;
\*\/                      return ECOMMENT;

[0-9]+\.[0-9]+\.[0-9]+  yylval = strdup(yytext); return THREE_NUMBERS;
[a-zA-Z0-9_\-]+         yylval = strdup(yytext); return WORD;
[a-zA-Z0-9\/\._\-]+     yylval = strdup(yytext); return FILENAME;
[a-zA-Z0-9_\-\*\.]+     yylval = strdup(yytext); return WILDCARD;
[a-zA-Z0-9\/\.,:_\-\+]+   yylval = strdup(yytext); return STUFF;
$\([a-zA-Z0-9_ ]+\)     yylval = strdup(yytext); return VARIABLE;

\n                      /* igore newlines */; 
[ \t]+                  /* ignore whitespace */;
%%
