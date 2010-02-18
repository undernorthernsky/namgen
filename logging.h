#ifndef LOGGING_H
#define LOGGING_H

typedef int (*log_fn_t)(const char *format, ...);
typedef int (*debug_fn_t)(const char*, const int, const char *format, ...);

extern log_fn_t info;
extern debug_fn_t debug;

#define DEBUG(fmt, args...) debug(__FILE__, __LINE__, fmt, ##args)

void set_debug_on(int t);

#endif
