#include <assert.h>
#include <stdlib.h>
#include "stringbuilder.h"
#include "template.h"
#include "utlist.h"


extern char *top_dir;

#define SECTION_VISIBLE 1

static void do_shared_keys(target_entry *target, ngt_dictionary *dict)
{
   if (target->compile_flags)
      ngt_set_string(dict, "TARGET_SPECIFIC_CF", target->compile_flags);
   if (target->link_flags)
      ngt_set_string(dict, "TARGET_SPECIFIC_LF", target->link_flags);
   // FIXME: target->libs
   if (target->src)
      ngt_set_string(dict, "SRC_EXPR", target->src);
   // FIXME: allow explicit src extension
   if (strstr(target->src, ".cpp"))
      ngt_set_string(dict, "SRC_EXTENSION", "cpp");
   else if (strstr(target->src, ".cxx"))
      ngt_set_string(dict, "SRC_EXTENSION", "cxx");
   else if (strstr(target->src, ".c"))
      ngt_set_string(dict, "SRC_EXTENSION", "c");

   if (!(target->other_flags & SKIP_INSTALL_MASK))
   {
      if (target->dest_sub_path)
      {
         ngt_set_string(dict, "DEST_SUB", target->dest_sub_path);
      } else
      {
         if (target->type == TYPE_PROGRAM)
         {
            ngt_set_string(dict, "DEST_SUB", "bin/");
         } else if (target->type == TYPE_LIBRARY)
         {
            ngt_set_string(dict, "DEST_SUB", "lib/");
         }
      }
      ngt_add_dictionary(dict, "IF_INSTALL", ngt_dictionary_new(), SECTION_VISIBLE);
   }
}

static void do_dependencies(target_entry *target, ngt_dictionary *dict)
{
   depend_list_entry *dep;
   stringbuilder *sb = sb_new();
   stringbuilder *include = sb_new();
   LL_FOREACH(target->dependencies, dep)
   {
      ngt_dictionary * dict_dep = ngt_dictionary_new();
      sb_reset(sb);
      sb_append_str(sb, dep->ptr->path_from_top);
      sb_append_str(sb, "/");
      sb_append_str(sb, dep->ptr->target_name);
      sb_append_str(include, " -I");
      sb_append_str(include, dep->ptr->path_from_top);
      char *p = sb_make_cstring(sb);
      ngt_set_string(dict_dep, "DEPENDENCY", p);
      free(p);
      ngt_add_dictionary(dict, "DEPENDS", dict_dep, SECTION_VISIBLE);
   }
   if (include->pos)
   {
      char *p = sb_make_cstring(include);
      ngt_set_string(dict, "INHERITED_INCLUDE", p);
      free(p);
   }
   sb_destroy(sb, 1);
   sb_destroy(include, 1);
}

static ngt_dictionary* dict_for_program_decl(target_entry *target)
{
   ngt_dictionary *dict = ngt_dictionary_new();
   ngt_set_string(dict, "PROGRAM_NAME", target->target_name);
   do_shared_keys(target, dict);
   do_dependencies(target, dict);
   return dict;
}

static ngt_dictionary* dict_for_library_decl(target_entry *target)
{
   ngt_dictionary *dict = ngt_dictionary_new();
   ngt_set_string(dict, "LIBRARY_NAME", target->target_name);
   do_shared_keys(target, dict);
   do_dependencies(target, dict);
   if (!(target->other_flags & SKIP_SHARED_MASK))
   {
      ngt_set_string(dict, "BUILD_THIS_AS_SHARED", "$(LIB_BUILD_SHARED)");
   }
   return dict;
}

ngt_dictionary* dict_for_module(module_entry *module)
{
   assert(module);
   ngt_dictionary *dict = ngt_dictionary_new();
   
   // global variables
   ngt_set_string(dict, "TOP_DIR", top_dir);
   ngt_set_string(dict, "THIS_DIR", module->dir_name);
   ngt_set_string(dict, "PATH_FROM_TOP", module->path_from_top);
   if (module->verbatim)
      ngt_set_string(dict, "VERBATIM", module->verbatim);

   target_entry *target;

   LL_FOREACH(module->targets, target)
   {
      ngt_dictionary *target_dict = NULL;
      switch(target->type)
      {
         case TYPE_PROGRAM:
            target_dict = dict_for_program_decl(target);
            break;
         case TYPE_LIBRARY:
            target_dict = dict_for_library_decl(target);
            break;
      }
      
      ngt_set_string(target_dict, "THIS_DIR", module->dir_name);
      ngt_set_string(target_dict, "PATH_FROM_TOP", module->path_from_top);

      if (target->type == TYPE_PROGRAM)
         ngt_add_dictionary(dict, "PROGRAM_RULES", target_dict, SECTION_VISIBLE);
      else if (target->type == TYPE_LIBRARY)
         ngt_add_dictionary(dict, "LIBRARY_RULES", target_dict, SECTION_VISIBLE);
   }

   return dict;
}
