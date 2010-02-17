#ifndef TARGET_H
#define TARGET_H

#include "uthash.h"

typedef enum target_type {
   TYPE_PROGRAM,
   TYPE_LIBRARY,
   TYPE_WORKER
} target_type;

struct target_entry;
struct target_lookup_entry;
struct depend_list_entry;
struct module_entry;

typedef struct depend_list_entry {
   char *str;
   struct target_entry *ptr;
   struct depend_list_entry *next;
} depend_list_entry;

#define SKIP_INSTALL_MASK 0x1
#define SKIP_SHARED_MASK  0x2
#define VERSION_INFO_SCHEMA 0x4
#define SKIP_STATIC_MASK 0x8

typedef struct target_entry {
   target_type type;
   char        *target_name;
   char        *dir_name;
   char        *directory;
   char        *path_from_top;
   char        *src;
   char        *extra_obj;
   char        *compile_flags;
   char        *link_flags;
   char        *lib_version_num;
   char        *libs;
   char        *dest_sub_path;
   char        *export_include;
   depend_list_entry *dependencies;

   int         other_flags;

   struct target_entry *next;
} target_entry;

typedef struct target_lookup_entry {
   char         *target_name;
   target_entry *ptr;

   UT_hash_handle hh;
} target_lookup_entry;

typedef struct module_entry {
   target_entry *targets;
   char         *dir_name;
   char         *directory;
   char         *path_from_top;
   char         *verbatim;

   struct module_entry *next;
} module_entry;

//extern string_list_entry* string_list_entry_new(const char *s);
//extern void string_list_entry_free(string_list_entry *e);
extern target_entry* target_entry_new(target_type t, const char *name);
extern void target_add_dependency(target_entry *e, const char *s);

extern void target_set_destdir_path(target_entry *e, const char *sub_path);

extern void target_set_skip_install(target_entry *e, const char *s);
extern void target_set_skip_shared(target_entry *e, const char *s);
extern void target_set_skip_static(target_entry *e, const char *s);

extern module_entry* module_entry_new(const char *name, char *directory, char *path_from_top);
extern target_entry* module_add_target(target_entry *e);

//extern void target_entry_free(target_entry *e);

extern module_entry* get_module_list(void); 

extern int resolve_all_dependencies(void);

extern void cleanup_data(void);
extern void print_target_decl(target_entry *e);

#endif
