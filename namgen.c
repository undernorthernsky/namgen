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

#include "ngtemplate/src/include/ngtemplate.h"

/* sub-sections */
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

static char *rules_suffix = ".rules";
static char *verbatim_suffix = ".verbatim";

static char *top_dir = NULL;
char * current_dir = NULL;


#define SECTION_VISIBLE 1
//const char *    tmpl_filename;
//ngt_dictionary* dictionary;

static void do_destdir_install(cfg_t *node, ngt_dictionary *dict)
{
    /* unless 'skip-install' is true for this target */
    if (!cfg_getbool(node, SKIP_INSTALL))
    {
        char *dd = cfg_getstr(node, DEST_DIR);
        if (dd)
        {
            ngt_set_string(dict, "DEST_SUB", dd);
            LOG_DEBUG("installing into DESTDIR/%s\n", dd);
        } else
        {
            LOG_DEBUG("%s\n", "installing into DESTDIR");
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
        LOG_DEBUG("dependency: %s\n", dep);
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
        LOG_DEBUG("Found wildcard 'src' expr: %s\n", buff);
        ngt_set_string(dict, "SRC_EXPR", buff);
        ngt_set_string(dict, "SRC_EXTENSION", (src + 2));
    } else
    {
        LOG_DEBUG("Assuming 'src' expr is a list: %s\n", src);
        /* assume a list */
        ngt_set_string(dict, "SRC_EXPR", src);
    }
}

static void do_project_specific_stuff(cfg_t *node, ngt_dictionary *dict)
{
    printf(".............\n");
    char *v = cfg_getstr(node, EXT_LIBS);
    if (v)
    {
        // FIXME: not supported yet
        LOG_DEBUG("WARN: feature %s is not yet supported\n", EXT_LIBS);
    }
    v = cfg_getstr(node, COMPILE_FLAGS);
    if (v)
    {
        ngt_set_string(dict, "TARGET_SPECIFIC_CF", v);
        LOG_DEBUG("adding %s\n", v);
    }
    v = cfg_getstr(node, LINK_FLAGS);
    if (v)
    {
        ngt_set_string(dict, "TARGET_SPECIFIC_LF", v);
        LOG_DEBUG("more-linking %s\n", v);
    }
}

static ngt_dictionary* make_program_rules_dict(cfg_t *node)
{
    ngt_dictionary *dict = ngt_dictionary_new();
    const char *name = cfg_title(node);
    ngt_set_string(dict, "PROGRAM_NAME", name);
    LOG_DEBUG("Processing program '%s'\n", name);
    
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
    LOG_DEBUG("Processing library '%s'\n", name);
    
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
    LOG_DEBUG("loading rules from '%s'\n", filename);
    int num_prog, num_lib, i;
    cfg_t *cfg = cfg_init(rules, CFGF_NOCASE);
    int ret = cfg_parse(cfg, filename);
    if(ret == CFG_PARSE_ERROR) {
        fprintf(stderr, "Failed to parse rules input file!\n");
        exit(1);
    }

    num_prog = cfg_size(cfg, PROGRAM_SECTION);
    num_lib  = cfg_size(cfg, LIBRARY_SECTION);
    printf("[%s:%i] file '%s' contains %i program rule(s) and %i library rule(s)\n",
            __FILE__, __LINE__, filename, num_prog, num_lib);

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
        ngt_add_dictionary(dict, "PROGRAM_RULES", dict_prog, SECTION_VISIBLE);
    }
    for (i=0;i<num_lib;i++)
    {
        cfg_t *cfg_prog = cfg_getnsec(cfg, LIBRARY_SECTION, i);
        assert(cfg_prog);
        ngt_dictionary *dict_prog = make_library_rules_dict(cfg_prog);
        assert(dict_prog);
        ngt_set_string(dict_prog, "THIS_DIR", current_dir);
        ngt_add_dictionary(dict, "LIBRARY_RULES", dict_prog, SECTION_VISIBLE);
    }

    cfg_free(cfg);

    return dict;
}


int process_directory(char * name)
{
    char *output_str = NULL;
    ngt_dictionary *dict = NULL;

    current_dir = name;
    size_t _max = strlen(rules_basename) + 9 + 1;
    char *fb = malloc(_max);
    snprintf(fb, _max, "%s%s", rules_basename, rules_suffix);
    if (access(fb, R_OK))
    {
        LOG_DEBUG("Directory '%s' does not contain '%s'\n", name, fb);
        goto pd_cleanup;
    }

    dict = make_rules_dict(fb);
    ngt_set_string(dict, "THIS_DIR", name);

    ngt_set_dictionary(template, dict);
    ngt_expand(template, &output_str); 
    int fd = open("makefile", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    write(fd, output_str, strlen(output_str));
    close(fd);

pd_cleanup:
    if (dict)
        ngt_dictionary_destroy(dict);
    if (output_str)
        free(output_str);
    if (fb)
        free(fb);
}

void iterate_directories(char * dirpath)
{
    char *old_pwd = NULL;

    old_pwd = get_current_dir_name();
    chdir(dirpath);
    process_directory(dirpath);

    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL)
    {
        fprintf(stderr, "Failed to opendir '%s'\n", dirpath);
        return;
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

    closedir(dir);
    chdir(old_pwd);
    free(old_pwd);
}

int main(int argc, char *argv[])
{
    char* output;

    template = ngt_new();
    top_dir = argv[1];
    ngt_load_from_filename(template, argv[2]);

    iterate_directories(argv[1]);

    /*
    ngt_dictionary* dictionary = ngt_dictionary_new();

    ngt_set_string(dictionary, "TOP-DIR", "Beer");

    ngt_dictionary *prog_rules_dict = ngt_dictionary_new();
    ngt_set_string(prog_rules_dict, "PROGRAM_NAME", "FooBar");

    ngt_add_dictionary(dictionary, "PROGRAM_RULES", prog_rules_dict, 1);



    ngt_set_dictionary(template, dictionary);
    ngt_expand(template, &output);

    fprintf(stdout, "%s\n", output);

    free(output);
    ngt_dictionary_destroy(dictionary);
    */
    if (template)
        ngt_destroy(template);
}
