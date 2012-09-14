#include <assert.h>
#include <stdlib.h>
#include "stringbuilder.h"
#include "template.h"
#include "utlist.h"
#include "uthash.h"
#include "myio.h"
#include "logging.h"


extern char *top_dir;

#define SECTION_VISIBLE 1

static char *srcdepends_tmpl_char = NULL;

void template_cleanup(void)
{
    if (srcdepends_tmpl_char)
        free(srcdepends_tmpl_char);
    srcdepends_tmpl_char = NULL;
}

static char* template_load_cb(const char *name)
{
    if (!strcmp(name, "src_depends"))
    {
        if (!srcdepends_tmpl_char)
            srcdepends_tmpl_char = load_template_string(name);
        return srcdepends_tmpl_char;
    } else
    {
        // if not a cached template just load it every time...
        return load_template_string(name);
    }

}

static void template_cleanup_cb(const char* name, char* template)
{
    // do nothing; free()ing causes 'double free' errors
}

const char* get_target_install_subdir(target_entry *e)
{
   if (e->dest_sub_path)
      return e->dest_sub_path;
   if (e->type == TYPE_PROGRAM)
   {
      return "bin";
   } else if (e->type == TYPE_LIBRARY)
   {
      return "lib";
   }
   return "";
}

static void do_shared_keys(target_entry *target, ngt_dictionary *dict)
{
   if (target->compile_flags)
      ngt_set_string(dict, "TARGET_SPECIFIC_CF", target->compile_flags);
   if (target->link_flags)
      ngt_set_string(dict, "TARGET_SPECIFIC_LF", target->link_flags);
   // FIXME: target->libs
   if (target->src)
   {
       ngt_set_string(dict, "SRC_EXPR", target->src);
       if (ngt_set_include_cb(dict, "src_depends", template_load_cb, template_cleanup_cb))
           fprintf(stderr, "ngt_set_include_cb failed!\n");
       ngt_add_dictionary(dict, "src_depends", ngt_dictionary_new(), SECTION_VISIBLE);
 
       if (target->src_ext)
       {
           ngt_set_string(dict, "SRC_EXTENSION", target->src_ext);
           ngt_set_string(dict, "LINK_CMD", "LINK_CXX"); // fixme: maybe not CXX?
       } else
       {
           if (strstr(target->src, ".cpp"))
           {
               ngt_set_string(dict, "SRC_EXTENSION", "cpp");
               ngt_set_string(dict, "LINK_CMD", "LINK_CXX"); 
           }
           else if (strstr(target->src, ".cxx"))
           {
               ngt_set_string(dict, "SRC_EXTENSION", "cxx");
               ngt_set_string(dict, "LINK_CMD", "LINK_CXX"); 
           }
           else if (strstr(target->src, ".cc"))
           {
               ngt_set_string(dict, "SRC_EXTENSION", "cc");
               ngt_set_string(dict, "LINK_CMD", "LINK_CXX"); 
           }
           else if (strstr(target->src, ".c"))
           {
               ngt_set_string(dict, "SRC_EXTENSION", "c");
               ngt_set_string(dict, "LINK_CMD", "LINK_CC"); 
           }
       }
   }
   if (target->cmd) {
       if (strstr(target->cmd, "g++"))
          ngt_set_string(dict, "LINK_CMD", "LINK_CXX");
       else if (strstr(target->cmd, "gcc"))
          ngt_set_string(dict, "LINK_CMD", "LINK_CC");
       else
          fprintf(stderr, "Warning: '%s' could not guess LINK_CMD for command: %s\n",
                  target->target_name, target->cmd);
   }
   if (target->extra_obj)
   {
       ngt_set_string(dict, "EXTRA_OBJ_EXPR", target->extra_obj);
   }

   if (!(target->other_flags & SKIP_INSTALL_MASK))
   {
      ngt_set_string(dict, "DEST_SUB", get_target_install_subdir(target));
      ngt_add_dictionary(dict, "IF_INSTALL", ngt_dictionary_new(), SECTION_VISIBLE);
   }
}

static void append_dep_include(target_entry *target, stringbuilder *include, target_lookup_entry **done)
{
    target_lookup_entry *_e = NULL;
    HASH_FIND_STR(*done, target->target_name, _e);
    if (_e)
    {
        DEBUG("Skipping '%s' - already done\n", target->target_name);
        return;
    }

    sb_append_str(include, " -I");
    if (target->export_include)
        sb_append_str(include, target->export_include);
    else
        sb_append_str(include, target->path_from_top);

    _e = malloc(sizeof(target_lookup_entry));
    _e->target_name = target->target_name; // sharing the cstr data!
    _e->ptr = NULL;
    HASH_ADD_KEYPTR(hh, *done, target->target_name, strlen(target->target_name), _e);

    depend_list_entry *dep;
    LL_FOREACH(target->dependencies, dep)
    {
        append_dep_include(dep->ptr, include, done);
    }
}

static void do_dependencies(target_entry *target, ngt_dictionary *dict)
{
   depend_list_entry *dep;
   stringbuilder *sb = sb_new();
   stringbuilder *include = sb_new();
   stringbuilder *sb_ext = sb_new();
   target_lookup_entry *include_done = NULL;
   int has_ext_depends = 0;
   LL_FOREACH(target->dependencies, dep)
   {
      ngt_dictionary * dict_dep = ngt_dictionary_new();
      sb_reset(sb);
      sb_append_str(sb, dep->ptr->path_from_top);
      sb_append_str(sb, "/");
      sb_append_str(sb, dep->ptr->target_name);
      append_dep_include(dep->ptr, include, &include_done);
      char *p = sb_make_cstring(sb);
      ngt_set_string(dict_dep, "DEPENDENCY", p);
      if (dep->ptr->type == TYPE_CMI)
      {
          sb_append_str(sb_ext, p);
          sb_append_str(sb_ext, ".la ");
          has_ext_depends = 1;
          DEBUG("%s depends on external: %s\n", target->target_name, dep->ptr->target_name);
      }
      free(p);
      /*
      if (!(dep->ptr->other_flags & SKIP_INSTALL_MASK))
      {
         ngt_dictionary * dict_dep_install = ngt_dictionary_new();
         ngt_set_string(dict_dep_install, "DEP_INSTALL_SUBPATH", get_target_install_subdir(dep->ptr));
         ngt_set_string(dict_dep_install, "DEP_NAME", dep->ptr->target_name);
         ngt_add_dictionary(dict, "DEPENDS_INSTALL", dict_dep_install, SECTION_VISIBLE);
      }*/
      char *depends_what = NULL;
      /* when only building .so or building both .so and .a use DEPENDS_SHARED */
      if ((dep->ptr->other_flags & SKIP_STATIC_MASK) ||
              ((dep->ptr->other_flags & (SKIP_STATIC_MASK | SKIP_SHARED_MASK | CONVENIENCE_MASK)) == 0))
              depends_what = "DEPENDS_SHARED";
      else
         depends_what = "DEPENDS_STATIC";
      ngt_add_dictionary(dict, depends_what, dict_dep, SECTION_VISIBLE);
      if (has_ext_depends)
      {
          ngt_dictionary * de_dict = ngt_dictionary_new();
          p = sb_make_cstring(sb_ext);
          ngt_set_string(de_dict, "DEPENDENCY", p);
          free(p);
          ngt_add_dictionary(dict, "DEPENDS_EXTERNAL", de_dict, SECTION_VISIBLE);
      }
   }
   if (include->pos)
   {
      char *p = sb_make_cstring(include);
      ngt_set_string(dict, "INHERITED_INCLUDE", p);
      free(p);
   }
   sb_destroy(sb, 1);
   sb_destroy(include, 1);
   sb_destroy(sb_ext, 1);
   target_lookup_entry *a;
   while (include_done)
   {
       a = include_done;
       HASH_DEL(include_done, a);
       free(a);
   }
}

static ngt_dictionary* dict_for_program_decl(target_entry *target)
{
   ngt_dictionary *dict = ngt_dictionary_new();
   ngt_set_string(dict, "TARGET_NAME", target->target_name);
   do_shared_keys(target, dict);
   do_dependencies(target, dict);
   return dict;
}

void make_dotted(char *s)
{
   for (char *p = s; *p != '\0'; p++)
   {
      if (*p == '.')
         *p = ':';
   }
}

static ngt_dictionary* dict_for_library_decl(target_entry *target)
{
   ngt_dictionary *dict = ngt_dictionary_new();
   ngt_set_string(dict, "TARGET_NAME", target->target_name);
   do_shared_keys(target, dict);
   do_dependencies(target, dict);
   stringbuilder *sb = sb_new();
   sb_append_strf(sb, "-rpath $(DESTDIR)/%s", get_target_install_subdir(target));
   if (target->other_flags & SKIP_SHARED_MASK)
   {
      /* only static lib => no -rpath */
      ngt_set_string(dict, "STATIC_OR_SHARED", "-static");
   } else if (target->other_flags & SKIP_STATIC_MASK)
   {
      /* only shared lib, needs -rpath */
      sb_append_str(sb, " -shared");
      sb_append_ch(sb, '\0');
      ngt_set_string(dict, "STATIC_OR_SHARED", sb_cstring(sb));
   } else if (target->other_flags & CONVENIENCE_MASK)
   {
      /* don't set anything: no -rpath, neither -shared nor -static */
      //DEBUG("%s is a convenience library\n", target->target_name);
   } else
   {
      /* build both, needs -rpath */
      sb_append_ch(sb, '\0');
      ngt_set_string(dict, "STATIC_OR_SHARED", sb_cstring(sb));
   }
   if (target->lib_version_num)
   {
      if (target->other_flags & VERSION_INFO_SCHEMA)
      {
         ngt_set_string(dict, "VERSION_SCHEMA", "-version-info");
      } else
      {
         ngt_set_string(dict, "VERSION_SCHEMA", "-version-number");
      }
      make_dotted(target->lib_version_num);
      ngt_set_string(dict, "VERSION_NUMBER", target->lib_version_num);
   }
   if (sb)
       sb_destroy(sb, 1);
   return dict;
}

static void strip_string_end(char *s, char *suffix)
{
    char *found = strstr(s, suffix);
    if (found)
        *found = 0;
}

static ngt_dictionary* dict_for_external_decl(target_entry *target)
{
   ngt_dictionary *dict = ngt_dictionary_new();
   ngt_set_string(dict, "TARGET_NAME", target->target_name);
   ngt_set_string(dict, "SRC_EXPR", target->src);
   if (target->src_ext) {
       ngt_set_string(dict, "EXTRACTED_DIRNAME", target->src_ext);
   } else {
       // try to guess from the filename
       char *tmp = strdup(rindex(target->src, '/') ? rindex(target->src, '/') + 1 : target->src);
       strip_string_end(tmp, ".tar.gz");
       strip_string_end(tmp, ".tar.bz2");
       DEBUG("Guessed archive-dir as [%s]\n", tmp);
       ngt_set_string(dict, "EXTRACTED_DIRNAME", tmp);
       free(tmp);
   }
   if (target->extra_obj)
       ngt_set_string(dict, "LIST_OF_PATCHES", target->extra_obj);
   if (target->cmd) {
       ngt_set_string(dict, "CMD", target->cmd);
   } else {
       ngt_set_string(dict, "CMD", "$(CONFIGURE_MAKE_INSTALL)");
   }

   depend_list_entry *dep;
   stringbuilder *sb = sb_new();
   LL_FOREACH(target->dependencies, dep)
   {
      ngt_dictionary * dict_dep = ngt_dictionary_new();
      sb_reset(sb);
      sb_append_str(sb, dep->ptr->path_from_top);
      sb_append_str(sb, "/");
      sb_append_str(sb, dep->ptr->target_name);
      char *p = sb_make_cstring(sb);
      ngt_set_string(dict_dep, "DEPENDENCY", p);
      free(p);
      ngt_add_dictionary(dict, "DEPENDS", dict_dep, SECTION_VISIBLE);
   }

   return dict;
}

ngt_dictionary* dict_for_module(module_entry *module)
{
   stringbuilder *ec_sb = NULL;
   assert(module);
   ngt_dictionary *dict = ngt_dictionary_new();
  
   // global variables
   ngt_set_string(dict, "TOP_DIR", top_dir);
   ngt_set_string(dict, "THIS_DIR", module->dir_name);
   ngt_set_string(dict, "PATH_FROM_TOP", module->path_from_top);
   if (module->verbatim)
   {
      ngt_dictionary *verb_dict = ngt_dictionary_new();
      ngt_set_string(verb_dict, "VERBATIM", module->verbatim);
      ngt_add_dictionary(dict, "HAS_VERBATIM", verb_dict, SECTION_VISIBLE);
   }

   target_entry *target;

   LL_FOREACH(module->targets, target)
   {
      if (target->extra_clean)
      {
          if (!ec_sb)
          {
            ec_sb = sb_new();
            sb_append_str(ec_sb, "\n\trm -f");
          }
          sb_append_str(ec_sb, " ");
          sb_append_str(ec_sb, target->extra_clean); 
      }
      ngt_dictionary *target_dict = NULL;
      switch(target->type)
      {
         case TYPE_PROGRAM:
            target_dict = dict_for_program_decl(target);
            break;
         case TYPE_LIBRARY:
            target_dict = dict_for_library_decl(target);
            break;
         case TYPE_WORKER:
            fprintf(stderr, "Program-bug: Got a TYPE_WORKER target in %s\n", __FUNCTION__);
            exit(1);
            break;
         case TYPE_CMI:
            target_dict = dict_for_external_decl(target);
            break;
      }
      
      ngt_set_string(target_dict, "THIS_DIR", module->dir_name);
      ngt_set_string(target_dict, "PATH_FROM_TOP", module->path_from_top);

      if (target->type == TYPE_PROGRAM)
         ngt_add_dictionary(dict, "PROGRAM_RULES", target_dict, SECTION_VISIBLE);
      else if (target->type == TYPE_LIBRARY)
         ngt_add_dictionary(dict, "LIBRARY_RULES", target_dict, SECTION_VISIBLE);
      else if (target->type == TYPE_CMI)
         ngt_add_dictionary(dict, "CMI_RULES", target_dict, SECTION_VISIBLE);
   }
   if (ec_sb)
   {
      sb_append_ch(ec_sb, '\0');
      ngt_set_string(dict, "EXTRA_CLEAN", sb_cstring(ec_sb));
      sb_destroy(ec_sb, 1);
   }

   return dict;
}
