%{
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
  
%}

%start declarations
%token PROGRAM LIBRARY WORKER
SRC DEPENDS FLAGS LIBS LDFLAGS ADD_OBJECTS DESTDIR SKIP_INSTALL SKIP_SHARED
TRUE_VALUE FALSE_VALUE VERSION_INFO VERSION_NUMBER THREE_NUMBERS
VARIABLE WORD WILDCARD FILENAME STUFF EXPR_MARK EQUALS QUOTE OBRACE EBRACE COMMENT_CHAR

%%
declarations:
            | declarations declaration
           ;

declaration:
           program_def | library_def | worker_def | comment_line
           ;

program_def:
           PROGRAM quoted_name
           {
               current_target = target_entry_new(TYPE_PROGRAM, $2);
           }
           def_content
           {
               module_add_target(current_target);
               current_target = NULL;
           }

library_def:
           LIBRARY quoted_name
           {
               current_target = target_entry_new(TYPE_LIBRARY, $2);
           }
           def_content
           {
               module_add_target(current_target);
               current_target = NULL;
           }

worker_def:
           WORKER quoted_name
           {
               current_target = target_entry_new(TYPE_WORKER, $2);
           }
           def_content
           {
               module_add_target(current_target);
               current_target = NULL;
           }

comment_line:
            COMMENT_CHAR comment_data
            ;

comment_data:
            | comment_data word_variable_filename_stuff
            ;

quoted_name:
           QUOTE WORD QUOTE
           {
              $$ = $2;
           }

def_content:
               OBRACE def_statements EBRACE
               ;

def_statements:
                  | def_statements def_statement
                  ;

def_statement:
                 src_statement ;
                 | depends_statement ;
                 | destdir_statement { current_target->dest_sub_path = strdup($1); }
                 | flags_statement ; 
                 | ld_flags_statement ;
                 | libs_statement ;
                 | version_num_statement ;
                 | add_objects_statement;
                 | skip_install_statement { target_set_skip_install(current_target, $1); }
                 | skip_shared_statement { target_set_skip_shared(current_target, $1); }
                 | comment_line ;

src_statement:
             SRC EQUALS { src_gatherer_reset(); }
             list_of_src_expr { current_target->src = src_gatherer_get_result(); }

add_objects_statement:
             ADD_OBJECTS EQUALS { src_gatherer_reset(); }
             list_of_src_expr { current_target->extra_obj = src_gatherer_get_result(); }


depends_statement:
                 DEPENDS EQUALS list_of_depends
                 ;

destdir_statement:
                 DESTDIR EQUALS FILENAME
                 {
                     $$ = $3;
                 }

version_num_statement:
                     vnum_schema EQUALS THREE_NUMBERS
                     { current_target->lib_version_num = strdup($3); }

vnum_schema:
           VERSION_NUMBER { current_target->other_flags &= ~VERSION_INFO_SCHEMA; }
           | VERSION_INFO { current_target->other_flags |= VERSION_INFO_SCHEMA; }

flags_statement:
               FLAGS EQUALS { sb_reset(sb); }
               list_of_stuff { current_target->compile_flags = sb_make_cstring(sb); }

ld_flags_statement:
                  LDFLAGS EQUALS { sb_reset(sb); }
                  list_of_stuff { current_target->link_flags = sb_make_cstring(sb); }

libs_statement:
              LIBS EQUALS { sb_reset(sb); }
              list_of_stuff { current_target->libs = sb_make_cstring(sb); }

skip_install_statement:
                      SKIP_INSTALL EQUALS true_false
                      { $$ = $3; }

skip_shared_statement:
                     SKIP_SHARED EQUALS true_false
                     { $$ = $3; }

true_false:
          TRUE_VALUE  { $$ = "true"; }
          | FALSE_VALUE { $$ = "false"; }

list_of_depends:
             WORD
             { target_add_dependency(current_target, $1); }
             | list_of_depends WORD
             { target_add_dependency(current_target, $2); }

list_of_src_expr:
                src_expr
                | list_of_src_expr src_expr
                ;

src_expr:
        WORD { src_gatherer_add_file($1); }
        | FILENAME { src_gatherer_add_file($1); }
        | STUFF { src_gatherer_add_file($1); }
        | WILDCARD { src_gatherer_add_wildcard($1); }
        | VARIABLE { src_gatherer_add_variable($1); }
        | EXPR_MARK { src_gather_set_explicit(); }

list_of_stuff:
             word_variable_filename_stuff
             { sb_append_str(sb, $1); sb_append_str(sb, " "); }
             | list_of_stuff word_variable_filename_stuff
             { sb_append_str(sb, $2); sb_append_str(sb, " "); }

word_variable_filename_stuff:
                            WORD { $$ = $1; }
                            | VARIABLE { $$ = $1; }
                            | FILENAME { $$ = $1; }
                            | STUFF { $$ = $1; }
