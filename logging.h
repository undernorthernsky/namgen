#ifndef LOGGING_H
#define LOGGING_H

typedef int (*log_fn_t)(const char *format, ...);

extern log_fn_t info;
extern log_fn_t debug;

void set_debug_on(int t);

#endif
