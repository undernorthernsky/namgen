#ifndef SRC_GATHERER_H
#define SRC_GATHERER_H

void src_gatherer_setup(char *path_from_top);
void src_gatherer_reset(void);
char* src_gatherer_get_result(void);
void src_gatherer_add_file(const char *f);
void src_gatherer_add_wildcard(const char *f);
void src_gatherer_add_variable(const char *v);
void src_gather_set_explicit(void);

#endif
