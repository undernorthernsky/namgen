#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "target.h"
#include "utlist.h"
#include "logging.h"

#define SF(x) if (x) free(x); x = NULL

#define TARGET_RESOLVED_MASK 0x20

static module_entry *all_modules = NULL;
static module_entry *current_module = NULL;
static target_lookup_entry *all_targets_hash = NULL;

extern int module_count;
extern int program_count;
extern int library_count;

static depend_list_entry* depend_list_entry_new(const char *s)
{
   depend_list_entry *e = malloc(sizeof(depend_list_entry));
   memset(e, 0, sizeof(depend_list_entry));
   e->str = strdup(s);
   e->ptr = NULL;
   return e;
}

static void depend_list_entry_free(depend_list_entry *e)
{
   SF(e->str);
   e->ptr = NULL;
   free(e);
}

void target_add_dependency(target_entry *e, const char *s)
{
   depend_list_entry *de = depend_list_entry_new(s);
   LL_APPEND(e->dependencies, de);
}

module_entry* module_entry_new(const char *name, char *directory, char *pft)
{
   module_entry *e = malloc(sizeof(module_entry));
   memset(e, 0, sizeof(module_entry));
   e->dir_name = strdup(name);
   e->directory = directory;
   e->path_from_top = strdup(pft);

   LL_APPEND(all_modules, e);
   current_module = e;
   ++module_count;

   return e;
}

target_entry* target_entry_new(target_type t, const char *name)
{
   target_entry *e = malloc(sizeof(target_entry));
   memset(e, 0, sizeof(target_entry));
   e->type = t;
   e->target_name = strdup(name);
   switch(e->type)
   {
      case TYPE_PROGRAM:
         ++program_count;
         break;
      case TYPE_LIBRARY:
         ++library_count;
         break;
      case TYPE_WORKER:
         break;
      default:
         break;
   }
   return e;
}

static void target_entry_free(target_entry *e)
{
   SF(e->target_name);
   e->dir_name = NULL;
   e->directory = NULL;
   e->path_from_top = NULL;
   SF(e->src);
   SF(e->extra_obj);
   SF(e->compile_flags);
   SF(e->link_flags);
   SF(e->libs);
   SF(e->dest_sub_path);
   depend_list_entry *a, *b;
   a = e->dependencies;
   while (a)
   {
      b = a->next;
      depend_list_entry_free(a);
      a = b;
   } 
   e->dependencies = NULL; 
   free(e);
}

static void module_entry_free(module_entry *e)
{
   target_entry *a, *b;
   a = e->targets;
   while (a)
   {
      b = a->next;
      target_entry_free(a);
      a = b;
   }
   SF(e->dir_name);
   SF(e->directory);
   SF(e->path_from_top);
   SF(e->verbatim);
   free(e);
}

static depend_list_entry* depend_list_copy(depend_list_entry *l)
{
    depend_list_entry *s, *d, *dh;
    s = l;
    dh = NULL;
    while (s)
    {
        d = depend_list_entry_new(s->str);
        LL_APPEND(dh, d);
        s = s->next;
    }
    return dh;
}

#define SC(s) if (prog->s) lib->s = strdup(prog->s);

static target_entry* libtarget_for_worker(target_entry *prog)
{
    char *name_as_lib = malloc(strlen(prog->target_name) + 4);
    sprintf(name_as_lib, "lib%s", prog->target_name);

    target_entry *lib = target_entry_new(TYPE_LIBRARY, name_as_lib);
    SC(src);
    SC(extra_obj);
    SC(compile_flags);
    SC(link_flags);
    SC(libs);
    lib->dest_sub_path = strdup("lib/embrace/");

    lib->dependencies = depend_list_copy(prog->dependencies);

    return lib;
}
#undef SC

target_entry* module_add_target(target_entry *e)
{
   // check for target name collision
   target_lookup_entry *_e = NULL;
   HASH_FIND_STR(all_targets_hash, e->target_name, _e);
   if (_e)
      return _e->ptr;

   if (e->type == TYPE_WORKER)
   {
       // register this decl twice
       e->type = TYPE_PROGRAM; // _this_ is a program
       ++program_count;

       // register a copy as a lib
       char *name_as_lib = malloc(strlen(e->target_name) + 4);
       sprintf(name_as_lib, "lib%s", e->target_name);
       // FIXME: name clashes of lib
       module_add_target(libtarget_for_worker(e));
   }

   // copy ptrs from module entry
   e->dir_name = current_module->dir_name;
   e->directory = current_module->directory;
   e->path_from_top = current_module->path_from_top;
   // add to current module
   LL_APPEND(current_module->targets, e);

   if (e->type == TYPE_LIBRARY)
   {
       // hash lib-names for dependency lookup
       _e = malloc(sizeof(target_lookup_entry));
       _e->target_name = e->target_name;
       _e->ptr = e;
       HASH_ADD_KEYPTR(hh, all_targets_hash, _e->target_name, strlen(_e->target_name), _e);
   }
   return NULL;
}

int resolve_all_dependencies(void)
{
   printf("Resolving module dependencies\n");
   int missing = 0;
   module_entry *me = NULL;
   LL_FOREACH(all_modules, me)
   {
      target_entry *te = NULL;
      LL_FOREACH(me->targets, te)
      {
         depend_list_entry *de = NULL;
         LL_FOREACH(te->dependencies, de)
         {
            target_lookup_entry *tle;

            HASH_FIND_STR(all_targets_hash, de->str, tle);
            if (tle)
            {
               debug("  Found '%s' dependency: %s\n", te->target_name, de->str);
               de->ptr = tle->ptr;
            } else
            {
               missing++;
               fprintf(stderr, "  Could not resolve '%s' requested by target '%s'\n",
                     de->str, te->target_name);
            }
         }
      }
   }

   return missing;
}

inline int is_true(const char *true_str)
{
   return !strcmp(true_str, "true");
}

void target_set_skip_install(target_entry *e, const char *true_str)
{
   if (is_true(true_str))
      e->other_flags |= SKIP_INSTALL_MASK;
}

void target_set_skip_shared(target_entry *e, const char *true_str)
{
   if (is_true(true_str))
      e->other_flags |= SKIP_SHARED_MASK;
}

module_entry* get_module_list(void)
{
   return all_modules;
}

void cleanup_data(void)
{
   // clear the lookup table
   target_lookup_entry *x;
   while (all_targets_hash)
   {
      x = all_targets_hash;
      HASH_DEL(all_targets_hash, x);
      free(x);
   }

   // clear all modules
   module_entry *a, *b;
   a = all_modules;
   while (a)
   {
      b = a->next;
      module_entry_free(a);
      a = b;
   }
   all_modules = NULL;
   current_module = NULL;
}

void print_target_decl(target_entry *e)
{
   printf("Target: %s [%s]\n", e->target_name,
         e->type == TYPE_PROGRAM ? "program" : "library");
   printf("  directory: %s\n\tshort: %s\n\tpath-from-top: %s\n",
         e->directory, e->dir_name, e->path_from_top);
   printf("  src: %s\n", e->src);
   if (e->compile_flags)
      printf("  compile-flags: %s\n", e->compile_flags);
   if (e->link_flags)
      printf("  link-flags: %s\n", e->link_flags);
   if (e->libs)
      printf("  libs: %s\n", e->libs);
   if (e->dest_sub_path)
      printf("  DESTDIR-sub: %s\n", e->dest_sub_path);
   if (e->other_flags & SKIP_INSTALL_MASK)
      printf("  skip-install: true\n");
   if (e->other_flags & SKIP_SHARED_MASK)
      printf("  skip-shared: true\n");

   if (e->dependencies)
   {
      printf("  dependencies: ");
      depend_list_entry *d;
      LL_FOREACH(e->dependencies, d)
      {
         printf("%s%s ", d->str, d->ptr ? "*" : "!");
      }
      printf("\n");
   }
}
