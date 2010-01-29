#define _GNU_SOURCE
#include <stdlib.h>
#include <confuse.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ngtemplate.h"

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

static char *rules_suffix = ".rules";
//static char *verbatim_suffix = ".verbatim";

static char *top_dir = NULL;
static char * current_dir = NULL;
static char * current_dir_path_from_top = NULL;


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
        // $(wildcard prj_a/*.cpp)
        char buff[512];
        snprintf(buff, 512, "$(wildcard %s/*.%s)", current_dir, (src + 2));
#ifndef NDEBUG
        printf("Found wildcard 'src' expr: %s\n", buff);
#endif
        ngt_set_string(dict, "SRC_EXPR", buff);
        ngt_set_string(dict, "SRC_EXTENSION", (src + 2));
    } else
    {
#ifndef NDEBUG
        LOG_DEBUG("Assuming 'src' expr is a list: %s\n", src);
#endif
        /* assume a list */
        ngt_set_string(dict, "SRC_EXPR", src);
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
            current_dir, filename, num_prog, num_lib);

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

char * get_subpath(char * from, char * to)
{
   char *f, *t;
   f = from;
   t = to;
   while (*f == *t)
   {
      f++;
      t++;
   }
   t++;
   return t;
}

int process_directory(char * name)
{
    int res = 0; 
    char *output_str = NULL;
    ngt_dictionary *dict = NULL;
    char *cwd = NULL;

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
    current_dir_path_from_top = get_subpath(top_dir, cwd);

    dict = make_rules_dict(fb);
    ngt_set_string(dict, "THIS_DIR", name);
    ngt_set_string(dict, "PATH_FROM_TOP", current_dir_path_from_top);

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

int main(int argc, char *argv[])
{
    template = ngt_new();

    top_dir = getenv("PWD"); 
    ngt_load_from_filename(template, argv[1]);

    iterate_directories(top_dir);

    if (template)
        ngt_destroy(template);
    return 0;
}
