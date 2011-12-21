#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "target.h"
#include "ngtemplate.h"

const char* get_target_install_subdir(target_entry *e);
ngt_dictionary* dict_for_module(module_entry *module);
void template_cleanup(void);

#endif
