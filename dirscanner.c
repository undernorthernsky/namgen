#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "stringbuilder.h"
#include "target.h"
#include "utlist.h"
#include "src_gatherer.h"
#include "logging.h"

static char *rules_basename = "build";
static char *rules_suffix = ".rules";

static char path_buf[1024];
extern char *top_dir;

static char *current_directory = NULL;

int module_count = 0;
int program_count = 0;
int library_count = 0;

void set_parsererror(int cause, int line, const char * msg)
{
   fprintf(stderr, "%s error reading %s/%s%s:%i near '%s'\n", 
         (cause == 0) ? "Lexer" : "Parser",
         current_directory,
         rules_basename, rules_suffix, line, msg);
   exit(1);
}

static char* get_cwd()
{
   int k = 512;
   char *buff = malloc(k);
   while (!getcwd(buff, k))
   {
      k *= 2;
      buff = realloc(buff, k);
   }
   return buff;
}

static char* get_path_from_top(char *top, char *to)
{
    // find common part
    char *f = top;
    char *t = to;
    while (*f == *t)
    {
        f++;
        t++;
    }
    if (*f == 0)
    {
        // simple subdir
        return ++t;
    } else
    {
        while (*f != '/')
            f--;
        while (*t != '/')
            t--;
        t++;
        stringbuilder *sb = sb_new();
        int from_remains = 0;
        while (*f != 0)
        {
            if (*f == '/')
            {
                from_remains = 1;
                sb_append_str(sb, "../");
            }
            f++;
        }
        sb_append_str(sb, t);
        memset(path_buf, 0, 1024);
        strncpy(path_buf, sb_cstring(sb), 1023);
        sb_destroy(sb, 1);
        return path_buf;
    }
}

static char* load_verbatim()
{
   struct stat st;
   stat("build.rules", &st);
   char *buff = malloc(st.st_size+1);
   memset(buff, 0, st.st_size+1);
   FILE *f = fopen("build.rules", "r");
   if (fread(buff, 1, st.st_size, f) != st.st_size)
      fprintf(stderr, "Error reading all expected bytes [%s]\n", __FUNCTION__);
   fclose(f);

   char *v = strstr(buff, "\nVERBATIM\n");
   if (v)
      v = strdup(v+10);
   else
      fprintf(stderr, "Failed to find expected VERBATIM section\n");
   free(buff);
   DEBUG("%s found verbatim data\n", current_directory);

   return v;
}

extern FILE* yyin;
extern int found_verbatim;
extern int yyparse(void);
extern int yylex_destroy(void);
extern void parser_setup(void);
extern void parser_cleanup(void);

static void parse_rules(module_entry *module)
{
   found_verbatim = 0;
   parser_setup();
   yyin = fopen("build.rules", "r");
   yyparse();
   if (found_verbatim)
   {
      module->verbatim = load_verbatim();
   }
   /* if 'VERBATIM' was found lex might still have input data in a buffer;
    * which would screw up any more parsing attempts... so cleanup */
   parser_cleanup();
   yylex_destroy();
}

static int process_directory(char * name, int work_mode)
{
   int res = 0; 
   char *current_dir = NULL;
   char *current_dir_path_from_top = NULL;

   if (rindex(name, '/'))
      current_dir = rindex(name, '/') + 1;
   else
      current_dir = name;
   size_t _max = strlen(rules_basename) + 9 + 1;
   char *fb = malloc(_max);
   snprintf(fb, _max, "%s%s", rules_basename, rules_suffix);
   if (access(fb, R_OK))
   {
      res = 1;
      goto pd_cleanup;
   }
   if (work_mode == -1) {
      if (!access("makefile", R_OK)) {
        unlink("makefile");
      }
      if (!access("makefile.ng", R_OK)) {
        unlink("makefile.ng");
      }
      goto pd_cleanup;
   }
   current_directory = get_cwd();
   current_dir_path_from_top = get_path_from_top(top_dir, current_directory);
   src_gatherer_setup(current_dir_path_from_top);
   
   module_entry *module = module_entry_new(current_dir, current_directory, current_dir_path_from_top);
   parse_rules(module);
   DEBUG("Loaded build.rules from %s\n", current_directory);

pd_cleanup:
   if (fb)
      free(fb);
   return res;
}

int iterate_directories(char * dirpath, int work_mode)
{
   char *old_pwd = NULL;
   int res = 0;
   old_pwd = get_current_dir_name();
   if (chdir(dirpath))
   {
      fprintf(stderr, "Error calling chdir(%s)\n", dirpath);
      res = 1;
      goto id_oops;
   }
   DEBUG("%s (%s)\n", __FUNCTION__, dirpath);
   process_directory(dirpath, work_mode);

   DIR *dir;
   struct dirent *entry;
   dir = opendir(".");
   if (dir == NULL)
   {
      fprintf(stderr, "Failed to opendir '%s'\n", dirpath);
      res = 1;
      goto id_cleanup;
   }
   while ((entry = readdir(dir)))
   {
      if (entry->d_type == DT_DIR)
      {
         if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
         iterate_directories(entry->d_name, work_mode);
      }
   }

id_cleanup:
   closedir(dir);
   if (chdir(old_pwd))
   {
      fprintf(stderr, "Failed to switch back to previous directory?\n");
   }
id_oops:
   free(old_pwd);
   return res;
}

#if 0
extern void install_bt_handler(void);

int main(int argc, char *argv[])
{
   install_bt_handler();
   top_dir = getenv("PWD");
   iterate_directories(top_dir);

   int missing = resolve_all_dependencies();
   if (!missing)
   {
      printf("Loaded: %i program rules and %i library rules in %i modules\n",
            program_count, library_count, module_count);
   } else
   {
      fprintf(stderr, "Error: missing %i dependencies\n", missing);
      goto cleanup;
   }

   module_entry *modules, *me;
   modules = get_module_list();
   LL_FOREACH(modules, me)
   {
      target_entry *te;
      LL_FOREACH(me->targets, te)
      {
         print_target_decl(te);
      }
      if (me->verbatim)
      {
         printf("Module '%s' has verbatim data:\n%s------------------------\n",
               me->dir_name, me->verbatim);
      }
   }

cleanup:
   cleanup_data();
}
#endif
