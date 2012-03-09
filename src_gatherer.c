#include <stdio.h>
#include "stringbuilder.h"
#include "logging.h"

static char *current_path_from_top = NULL;
static stringbuilder *src_sb = NULL;
static int is_explicit_mode = 0;

void src_gatherer_setup(char *path_from_top)
{
   current_path_from_top = path_from_top;
   if (!src_sb)
      src_sb = sb_new();
}

void src_gatherer_reset(void)
{
   debug("%s\n", __FUNCTION__);
   sb_reset(src_sb);
   is_explicit_mode = 0;
}

char* src_gatherer_get_result(void)
{
   char * src = sb_make_cstring(src_sb);
   debug("%s: %s\n", __FUNCTION__, src);
   return src;
}

void src_gatherer_add_file(const char *f)
{
   debug("%s: %s\n", __FUNCTION__, f);
   if (is_explicit_mode)
      sb_append_str(src_sb, f);
   else
   {
      sb_append_str(src_sb, current_path_from_top);
      sb_append_str(src_sb, "/");
      sb_append_str(src_sb, f);
   }
   sb_append_str(src_sb, " ");
}

void src_gatherer_add_wildcard(const char *f)
{
   debug("%s: %s\n", __FUNCTION__, f);
   sb_append_str(src_sb, "$(wildcard ");
   sb_append_str(src_sb, current_path_from_top);
   sb_append_str(src_sb, "/");
   sb_append_str(src_sb, f);
   sb_append_str(src_sb, ") ");
}

void src_gatherer_add_variable(const char *v)
{
   debug("%s: %s\n", __FUNCTION__, v);
   sb_append_str(src_sb, v);
   sb_append_str(src_sb, " ");
}

void src_gather_set_explicit(void)
{
   debug("%s\n", __FUNCTION__);
   is_explicit_mode = 1;
}
