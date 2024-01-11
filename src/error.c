#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ERROR_CODE_STRING[] = {
    [ERROR_NO_ERROR]                = "ERROR_NO_ERROR",
    [ERROR_NO_MEMORY]               = "ERROR_NO_MEMORY",
    [ERROR_EXPRESSION_EMTPY]        = "ERROR_EXPRESSION_EMTPY",
    [ERROR_EXCESS_DECIMAL_DELIM]    = "ERROR_EXCESS_DECIMAL_DELIM",
    [ERROR_UNKNOWN_CHARACTER]       = "ERROR_UNKNOWN_CHARACTER",
    [ERROR_UNKNOWN_EXPRESSION]      = "ERROR_UNKNOWN_EXPRESSION",
    [ERROR_STACK_EMPTY]             = "ERROR_STACK_EMPTY",
    [ERROR_QUEUE_EMPTY]             = "ERROR_QUEUE_EMPTY",
    [ERROR_MISMATCHED_PARENTHESES]  = "ERROR_MISMATCHED_PARENTHESES",
    [ERROR_LEADING_OPERATOR]        = "ERROR_LEADING_OPERATOR",
    [ERROR_CALCULATION]             = "ERROR_CALCULATION",
};

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

void exit_on_error(error_t e)
{
    if (e.code == ERROR_NO_ERROR) return;

    printf("%s: %s\n", ERROR_CODE_STRING[e.code], e.msg);
    exit(e.code);
}