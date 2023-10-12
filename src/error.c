#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

error_t error(int code, const char fmt[], ...)
{
    error_t e;
    e.code = code;

    if (fmt == NULL) {
        e.msg[0] = 0;
        return e;
    }
    va_list args;
    va_start(args, fmt);
    vsprintf(e.msg, fmt, args);
    va_end(args);    
    
    return e;
}

void log_on_error(error_t e)
{
    if (e.code == ERROR_NO_ERROR) return;

    printf("ERROR %d: %s\n", e.code, e.msg);
    exit(e.code);
}