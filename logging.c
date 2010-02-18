#include <stdio.h>
#include <stdarg.h>
#include "logging.h"

static int dummy_fn(const char *file, const int line, const char *fmt, ...)
{
    return 0;
}

static int debug_fn(const char* file, const int line, const char *fmt, ...)
{
    va_list ap;
    int ret = 0;
    printf("DEBUG [%s:%i]: ", file, line);
    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

log_fn_t info = printf;
debug_fn_t debug = dummy_fn;

void set_debug_on(int t)
{
    if (t)
        debug = debug_fn;
    else
        debug = dummy_fn;
}
