#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "myio.h"
#include "utlist.h"
#include "ngtemplate.h"
#include "stringbuilder.h"
#include "dirscanner.h"
#include "target.h"
#include "template.h"
#include "logging.h"

static ngt_template *template = NULL;
static char *template_file = COMPILED_IN_TEMPLATE_FILE; 

char *top_dir = NULL;

static int process_module(module_entry *module)
{
   DEBUG("Creating %s/makefile\n", module->directory);
   char *output_str = NULL;
   int res = 0;

   if (chdir(module->directory))
      return 1;
   ngt_dictionary *dict = dict_for_module(module);
   ngt_set_dictionary(template, dict);
   ngt_expand(template, &output_str);
   int fd = open("makefile", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
   int wl = strlen(output_str);
   if (write(fd, output_str, wl) != wl)
   {
      fprintf(stderr, "Error while writing '%s/makefile'\n",
            module->directory);
      res = 1;
   }
   close(fd);
   if (dict)
      ngt_dictionary_destroy(dict);
   if (output_str)
      free(output_str);

   return res;
}

static void usage(const char * prg)
{
    char *_prg = strdup(prg);
    char *name = basename(_prg);
    printf("USAGE: %s [flags]\n", name);
    free(_prg);
    name = load_file("usage.txt");
    if (name)
    {
        printf("%s\n", name);
        free(name);
    } else
    {
        fprintf(stderr, "Failed to load usage.txt\n");
    }
}

static void show_documentation(void)
{
    char *doc = load_file("documentation.txt");
    if (doc)
    {
        printf("%s\n", doc);
        free(doc);
    } else
    {
        fprintf(stderr, "Failed to load documentation.txt\n");
    }
}

struct charp_list_entry;
typedef struct charp_list_entry {
    char *dir;
    struct charp_list_entry *next;
} charp_list_entry;

static charp_list_entry* make_list_entry(char *dir)
{
    charp_list_entry *e = malloc(sizeof(charp_list_entry));
    int k = strlen(dir) - 1;
    if (dir[k] == '/')
       dir[k] = 0;
    e->dir = dir;
    e->next = NULL;
    return e;
}


#define SB_APPEND(sb, s)  sb_append_str(sb, " "); sb_append_str(sb, me->dir_name); sb_append_str(sb, s);

static void create_master_include(void)
{
    if (chdir(top_dir))
    {
        fprintf(stderr, "Could not change to dir: %s\n", top_dir);
        return;
    }
    FILE *f = fopen("makefile.dirs", "w");
    fprintf(f, "# This file was generated by namgen; do not edit!\n\n");

    stringbuilder *sb_clean = sb_new();
    stringbuilder *sb_install = sb_new();
    stringbuilder *sb_uninstall = sb_new();

    module_entry *modules, *me;
    modules = get_module_list();
    LL_FOREACH(modules, me)
    {
        fprintf(f, "include %s/makefile\n", me->path_from_top);
        SB_APPEND(sb_clean, "_clean");
        SB_APPEND(sb_install, "_install");
        SB_APPEND(sb_uninstall, "_uninstall");
    }
    fprintf(f, "\n");
    sb_append_ch(sb_clean, '\0');
    sb_append_ch(sb_install, '\0');
    sb_append_ch(sb_uninstall, '\0');

    fprintf(f, "clean: %s\n", sb_cstring(sb_clean));
    fprintf(f, "install: %s\n", sb_cstring(sb_install));
    fprintf(f, "uninstall: %s\n", sb_cstring(sb_uninstall));

    fprintf(f, ".PHONY: %s %s %s\n",
          sb_cstring(sb_clean),
          sb_cstring(sb_install),
          sb_cstring(sb_uninstall));

    sb_destroy(sb_clean, 1);
    sb_destroy(sb_install, 1);
    sb_destroy(sb_uninstall, 1);

    fclose(f);
}

static void print_loaded_rules(void)
{
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
}

extern void install_bt_handler(void);

int main(int argc, char *argv[])
{
    int c, opt_idx;
    int print_rules = 0;
    static struct option long_opts[] = {
        {"add-dir",  1, 0, 'a'},
        {"print",    0, 0, 'p'},
        {"help",     0, 0, 'h'},
        {"show-doc", 0, 0, 'H'},
        {"debug",    0, 0, 'd'},
        {0, 0, 0, 0}
    };
    charp_list_entry *additional_dir_list_head = NULL; 
    if (io_init(argv[0]))
    {
        fprintf(stderr, "Error loading IO subsystem for %s\n", argv[0]);
        exit(1);
    }

    while ((c = getopt_long(argc, argv, "a:dph?H", long_opts, &opt_idx)) != -1) {
        switch(c) {
            case 'a':
                LL_APPEND(additional_dir_list_head, make_list_entry(optarg));
                break;
            case 'p':
                print_rules = 1;
                break;
            case 'd':
                set_debug_on(1);
                break;
            case 'h':
            case '?':
                usage(argv[0]);
                exit(0);
                break;
            case 'H':
                show_documentation();
                exit(0);
                break;
            default:
                usage(argv[1]);
                exit(1);
        }
    };

    install_bt_handler();
    
    top_dir = getenv("PWD"); 

    iterate_directories(top_dir);
    if (additional_dir_list_head)
    {
        charp_list_entry *e, *n;
        LL_FOREACH(additional_dir_list_head, e)
        {
            iterate_directories(e->dir);
        }

        // cleanup
        e = additional_dir_list_head;
        while (e) {
            n = e->next;
            free(e);
            e = n;
        }
    }

    int missing = resolve_all_dependencies();
    if (!missing)
    {
       printf("Loaded: %i program rules and %i library rules in %i modules\n",
             program_count, library_count, module_count);
    } else
    {
       fprintf(stderr, "Error: missing %i dependencies\n", missing);
       if (!print_rules)
          exit(1);
    }

    if (print_rules)
    {
       print_loaded_rules();
       goto work_skipped;
    }

    template = ngt_new();

    if (load_template(template, template_file))
    {
        fprintf(stderr, "Error loading template %s\n", template_file);
        goto work_skipped;
    }

    module_entry *modules, *me;
    modules = get_module_list();
    LL_FOREACH(modules, me)
    {
       if (process_module(me))
          goto work_skipped;
    }

    create_master_include();

work_skipped:
    cleanup_data();
    template_cleanup();
    if (template)
        ngt_destroy(template);

    io_quit();
    return 0;
}
