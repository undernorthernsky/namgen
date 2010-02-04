#define _GNU_SOURCE
#include <stdlib.h>
#include <confuse.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utlist.h"
#include "ngtemplate.h"
#include "stringbuilder.h"

#define PROGRAM_SECTION "program"
#define LIBRARY_SECTION "library"

#define COMPILE_FLAGS "flags"
#define LINK_FLAGS    "ld-flags"

#define DEPENDS  "depends"
#define EXT_LIBS "libs"
#define SRC_EXPR "src"
#define DEST_DIR "dest-dir"

#define SKIP_INSTALL "skip-install"
#define SKIP_SHARED  "skip-shared"

#define LOG_DEBUG printf

static cfg_opt_t program_rules[] = {
    CFG_STR(SRC_EXPR, 0, CFGF_NONE),
    CFG_STR_LIST(DEPENDS, 0, CFGF_NONE),
    CFG_STR(DEST_DIR, 0, CFGF_NONE),

    CFG_STR(COMPILE_FLAGS, 0, CFGF_NONE),
    CFG_STR(EXT_LIBS, 0, CFGF_NONE),
    CFG_STR(LINK_FLAGS, 0, CFGF_NONE),

    CFG_BOOL(SKIP_INSTALL, cfg_false, CFGF_NONE),

    CFG_END()
};

static cfg_opt_t library_rules[] = {
    CFG_STR(SRC_EXPR, 0, CFGF_NONE),
    CFG_STR_LIST(DEPENDS, 0, CFGF_NONE),
    CFG_STR(DEST_DIR, 0, CFGF_NONE),

    CFG_STR(COMPILE_FLAGS, 0, CFGF_NONE),
    CFG_STR(EXT_LIBS, 0, CFGF_NONE),
    CFG_STR(LINK_FLAGS, 0, CFGF_NONE),

    CFG_BOOL(SKIP_INSTALL, cfg_false, CFGF_NONE),
    CFG_BOOL(SKIP_SHARED,  cfg_false, CFGF_NONE),

    CFG_END()
};

static cfg_opt_t rules[] = {
    CFG_STR(COMPILE_FLAGS, 0, CFGF_NONE),
    CFG_STR(LINK_FLAGS, 0, CFGF_NONE),
    CFG_STR(EXT_LIBS, 0, CFGF_NONE),

    CFG_SEC(PROGRAM_SECTION, program_rules, CFGF_MULTI | CFGF_TITLE),
    CFG_SEC(LIBRARY_SECTION, library_rules, CFGF_MULTI | CFGF_TITLE),
    CFG_END()
};


static ngt_template *template = NULL;
static char *rules_basename = "build";

static char *template_file = COMPILED_IN_TEMPLATE_FILE; 

static char *rules_suffix = ".rules";
//static char *verbatim_suffix = ".verbatim";

static char *top_dir = NULL;
static char *current_dir = NULL;
static char *current_dir_path_from_top = NULL;

struct sub_entry;
typedef struct sub_entry {
    char *name_clean_rule;
    char *path_from_top;
    struct sub_entry *next;
    struct sub_entry *prev;
} sub_entry;

static sub_entry *sub_list_head = NULL;

#define SECTION_VISIBLE 1

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

static void do_destdir_install(cfg_t *node, ngt_dictionary *dict)
{
    /* unless 'skip-install' is true for this target */
    if (!cfg_getbool(node, SKIP_INSTALL))
    {
        char *dd = cfg_getstr(node, DEST_DIR);
        if (dd)
        {
            ngt_set_string(dict, "DEST_SUB", dd);
#ifndef NDEBUG
            printf("installing into DESTDIR/%s\n", dd);
#endif
        } else
        {
#ifndef NDEBUG
            printf("%s\n", "installing into DESTDIR");
#endif
        }

        ngt_add_dictionary(dict, "IF_INSTALL", ngt_dictionary_new(), SECTION_VISIBLE);
    }
}

static void do_dependencies(cfg_t *node, ngt_dictionary *dict)
{
    int i;
    int nd = cfg_size(node, DEPENDS);
    for (i=0;i<nd;i++)
    {
        const char *dep = cfg_getnstr(node, DEPENDS, i);
#ifndef NDEBUG
        printf("dependency: %s\n", dep);
#endif
        ngt_dictionary * dict_dep = ngt_dictionary_new();
        ngt_set_string(dict_dep, "DEPENDENCY", dep);
        ngt_add_dictionary(dict, "DEPENDS", dict_dep, SECTION_VISIBLE);
    }
}

static stringbuilder* split_list_into_sb(char *src)
{
   stringbuilder *sb = NULL;
   char *start, *next, *last;
   int len;

   start = src;
   last = src + strlen(src) - 1;

#ifndef NDEBUG
   printf("input [%s]\n", src);
#endif
   if (start > last)
   {
      fprintf(stderr, "%s\n", "WARN: Empty 'src' input");
      return NULL;
   }
   
   sb = sb_new();
   if (start == last)
   {
      sb_append_str(sb, current_dir_path_from_top);
      sb_append_ch(sb, '/');
      sb_append_strn(sb, start, 1);
      sb_append_ch(sb, ' ');
   }

   while (start < last)
   {
      for (; *start == ' '; start++);
      for (next = start + 1; (*next != ' ') && (next <= last); next++);
      len = next - start;
      sb_append_str(sb, current_dir_path_from_top);
      sb_append_str(sb, "/");
      sb_append_strn(sb, start, len);
      sb_append_str(sb, " ");
      start = next;
   }
   return sb;
}

static void do_src_expr(cfg_t *node, ngt_dictionary *dict, const char *name)
{
    char *src = cfg_getstr(node, SRC_EXPR);
    if (!src)
    {
        fprintf(stderr, "Error in program-section %s: No 'src' declaration!\n", name);
        return;
    }
    
    if (!strncmp(src, "*.", 2))
    {
        /* wildcard */
        char buff[512];
        snprintf(buff, 512, "$(wildcard %s/*.%s)", current_dir_path_from_top, (src + 2));
#ifndef NDEBUG
        printf("Found wildcard 'src' expr: %s\n", buff);
#endif
        ngt_set_string(dict, "SRC_EXPR", buff);
        ngt_set_string(dict, "SRC_EXTENSION", (src + 2));
    } else
    {
       char *buff = NULL;
       if (src[0] == '!')
       {
#ifndef NDEBUG
          LOG_DEBUG("Assuming 'src' expr is to be copied verbatim: %s\n", src);
#endif
          /* assume a list */
          ngt_set_string(dict, "SRC_EXPR", src + 1);
       } else
       {
#ifndef NDEBUG
          LOG_DEBUG("Assuming files in 'src' expr are to be adjusted to %s\n", current_dir_path_from_top); 
#endif
          stringbuilder *sb = split_list_into_sb(src);
          if (sb)
          {
            printf("output [%s]\n", sb_cstring(sb));
            ngt_set_string(dict, "SRC_EXPR", sb_cstring(sb));
            sb_destroy(sb, 1);
          }
       }
       if (strstr(src, ".cpp"))
          buff = "cpp";
       else if (strstr(src, ".cxx"))
          buff = "cxx";
       else if (strstr(src, ".c"))
          buff = "c";
#ifndef NDEBUG
       printf("Guessed src extension: %s\n", buff);
#endif
       if (!buff)
          fprintf(stderr, "WARN: no src extension detected in input: %s\n", src);
       ngt_set_string(dict, "SRC_EXTENSION", buff);
    }
}

static void do_project_specific_stuff(cfg_t *node, ngt_dictionary *dict)
{
    char *v = cfg_getstr(node, EXT_LIBS);
    if (v)
    {
        // FIXME: not supported yet
        fprintf(stderr, "WARN: feature %s is not yet supported\n", EXT_LIBS);
    }
    v = cfg_getstr(node, COMPILE_FLAGS);
    if (v)
    {
        ngt_set_string(dict, "TARGET_SPECIFIC_CF", v);
    }
    v = cfg_getstr(node, LINK_FLAGS);
    if (v)
    {
        ngt_set_string(dict, "TARGET_SPECIFIC_LF", v);
    }
}

static ngt_dictionary* make_program_rules_dict(cfg_t *node)
{
    ngt_dictionary *dict = ngt_dictionary_new();
    const char *name = cfg_title(node);
    ngt_set_string(dict, "PROGRAM_NAME", name);
    printf("Processing program '%s'\n", name);
    
    /* required keys */

    do_src_expr(node, dict, name);

    /* optional keys */

    do_project_specific_stuff(node, dict);

    do_dependencies(node, dict);
    
    do_destdir_install(node, dict);

    return dict;
}


static ngt_dictionary* make_library_rules_dict(cfg_t *node)
{
    ngt_dictionary *dict = ngt_dictionary_new();
    const char *name = cfg_title(node);
    ngt_set_string(dict, "LIBRARY_NAME", name);
    printf("Processing library '%s'\n", name);
    
    /* required keys */

    do_src_expr(node, dict, name);


    /* optional keys */

    do_project_specific_stuff(node, dict);
    if (!cfg_getbool(node, SKIP_SHARED))
    {
        ngt_set_string(dict, "BUILD_THIS_AS_SHARED", "$(LIB_BUILD_SHARED)");
    }

    do_dependencies(node, dict);
    
    do_destdir_install(node, dict);

    return dict;
}


#define TRY_SHARED_KEY(kv, cfg, key, dict, out) kv = cfg_getstr(cfg, key); \
if (kv) \
    ngt_set_string(dict, out, kv);

static ngt_dictionary* make_rules_dict(const char *filename)
{
    assert(filename);
    int num_prog, num_lib, i;
    cfg_t *cfg = cfg_init(rules, CFGF_NOCASE);
    int ret = cfg_parse(cfg, filename);
    if(ret == CFG_PARSE_ERROR) {
        fprintf(stderr, "Failed to parse rules input file!\n");
        exit(1);
    }

    num_prog = cfg_size(cfg, PROGRAM_SECTION);
    num_lib  = cfg_size(cfg, LIBRARY_SECTION);
    printf("File '%s/%s' contains %i program rule(s) and %i library rule(s)\n",
            current_dir_path_from_top, filename, num_prog, num_lib);

    ngt_dictionary *dict = ngt_dictionary_new();
    assert(dict);
    ngt_set_string(dict, "TOP_DIR", top_dir);

    /* read globals */
    
    char * str_val;
    TRY_SHARED_KEY(str_val, cfg, COMPILE_FLAGS, dict, "CXXFLAGS");
    TRY_SHARED_KEY(str_val, cfg, LINK_FLAGS,    dict, "LDFLAGS");

    for (i=0;i<num_prog;i++)
    {
        cfg_t *cfg_prog = cfg_getnsec(cfg, PROGRAM_SECTION, i);
        assert(cfg_prog);
        ngt_dictionary *dict_prog = make_program_rules_dict(cfg_prog);
        assert(dict_prog);
        ngt_set_string(dict_prog, "THIS_DIR", current_dir);
        ngt_set_string(dict_prog, "PATH_FROM_TOP", current_dir_path_from_top);
        ngt_add_dictionary(dict, "PROGRAM_RULES", dict_prog, SECTION_VISIBLE);
    }
    for (i=0;i<num_lib;i++)
    {
        cfg_t *cfg_prog = cfg_getnsec(cfg, LIBRARY_SECTION, i);
        assert(cfg_prog);
        ngt_dictionary *dict_prog = make_library_rules_dict(cfg_prog);
        assert(dict_prog);
        ngt_set_string(dict_prog, "THIS_DIR", current_dir);
        ngt_set_string(dict_prog, "PATH_FROM_TOP", current_dir_path_from_top);
        ngt_add_dictionary(dict, "LIBRARY_RULES", dict_prog, SECTION_VISIBLE);
    }

    cfg_free(cfg);

    return dict;
}

static char path_buf[1024];

char* get_path_from_top(char *top, char *to)
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

void remember_entries(char * name, char * dir)
{
    sub_entry *e = malloc(sizeof(sub_entry));
    char *tmp = malloc(strlen(name) + 7);
    sprintf(tmp, "%s_clean", name);
    e->name_clean_rule = tmp;
    tmp = malloc(strlen(dir) + 10);
    sprintf(tmp, "%s/makefile", dir);
    e->path_from_top = tmp;
    DL_APPEND(sub_list_head, e);
}

int process_directory(char * name)
{
#ifndef NDEBUG
   printf("processing directory: %s\n", name);
#endif
    int res = 0; 
    char *output_str = NULL;
    ngt_dictionary *dict = NULL;
    char *cwd = NULL;

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
    cwd = get_cwd();
    current_dir_path_from_top = get_path_from_top(top_dir, cwd);
    remember_entries(current_dir, current_dir_path_from_top);
#ifndef NDEBUG
   printf("cwd [%s], pft [%s]\n", cwd, current_dir_path_from_top);
#endif

    dict = make_rules_dict(fb);
    ngt_set_string(dict, "THIS_DIR", current_dir);
    ngt_set_string(dict, "PATH_FROM_TOP", current_dir_path_from_top);

#ifndef NDEBUG
    printf("expanding template\n");
#endif
    ngt_set_dictionary(template, dict);
    ngt_expand(template, &output_str); 
    int fd = open("makefile", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    int wl = strlen(output_str);
    if (write(fd, output_str, wl) != wl)
    {
       fprintf(stderr, "Error while writing 'makefile'\n");
       res = 1;
    }
    close(fd);
    printf("%s\n", "----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----");

pd_cleanup:
    if (cwd)
       free(cwd);
    if (dict)
        ngt_dictionary_destroy(dict);
    if (output_str)
        free(output_str);
    if (fb)
        free(fb);
    return res;
}

int iterate_directories(char * dirpath)
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
    process_directory(dirpath);

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
            iterate_directories(entry->d_name);
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

void usage(const char * prg)
{
    printf("USAGE: %s [flags]\n", prg);
}

struct charp_list_entry;
typedef struct charp_list_entry {
    char *dir;
    struct charp_list_entry *next;
} charp_list_entry;

static charp_list_entry* make_list_entry(char *dir)
{
    charp_list_entry *e = malloc(sizeof(charp_list_entry));
    e->dir = dir;
    e->next = NULL;
    return e;
}

static void create_master_include(void)
{
    if (chdir(top_dir))
    {
        fprintf(stderr, "Could not change to dir: %s\n", top_dir);
        return;
    }
    FILE *f = fopen("makefile.dirs", "w");
    fprintf(f, "# This file was generated by namgen; do not edit!\n\n");

    stringbuilder *sb = sb_new();
    sb_append_str(sb, "clean:");
    sub_entry *entry;
    DL_FOREACH(sub_list_head, entry)
    {
        fprintf(f, "include %s\n", entry->path_from_top);
        sb_append_str(sb, " ");
        sb_append_str(sb, entry->name_clean_rule);
    }
    fprintf(f, "\n");

    fprintf(f, "%s\n", sb_cstring(sb));
    sb_destroy(sb, 1);
    fprintf(f, "\trm -f $(TARGETS_TO_BUILD)\n");
    fprintf(f, "\trm -f $(TARGETS_TO_INSTALL)\n");

    fclose(f);
}

int main(int argc, char *argv[])
{
    int c, opt_idx;
    static struct option long_opts[] = {
        {"add-dir", 1, 0, 'a'},
        {0, 0, 0, 0}
    };
    charp_list_entry *additional_dir_list_head = NULL; 

    while ((c = getopt_long(argc, argv, "a:h?", long_opts, &opt_idx)) != -1) {
        switch(c) {
            case 'a':
                LL_PREPEND(additional_dir_list_head, make_list_entry(optarg));
                break;
            case 'h':
            case '?':
                usage(argv[0]);
                exit(0);
                break;
            default:
                usage(argv[1]);
                exit(1);
        }
    };

    template = ngt_new();

    top_dir = getenv("PWD"); 
    ngt_load_from_filename(template, template_file);

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

    create_master_include();

    sub_entry *entry = sub_list_head;
    sub_entry *next;
    while (entry)
    {
        next = entry->next;
        free(entry->name_clean_rule);
        free(entry->path_from_top);
        free(entry);
        entry = next;
    }

    if (template)
        ngt_destroy(template);
    return 0;
}
