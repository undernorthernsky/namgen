#include <stdio.h>
#include "logging.h"

static int dummy_fn(const char *fmt, ...)
{
    return 0;
}

log_fn_t info = printf;
log_fn_t debug = dummy_fn;

void set_debug_on(int t)
{
    if (t)
        debug = printf;
    else
        debug = dummy_fn;
}
