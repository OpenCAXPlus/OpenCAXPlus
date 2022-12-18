#include "log.h"
#include <stdarg.h>
#include <stdio.h>

void ocplog_info(const char* fmt, ...)
{
    va_list args;
    char    temp[512] = {'\0'};
    sprintf(temp, "%s: %s\n", "INFO", fmt);
    va_start(args, fmt);
    vprintf(temp, args);
    va_end(args);
}

void ocplog_debug(const char* fmt, ...)
{
    va_list args;
    char    temp[512] = {'\0'};
    sprintf(temp, "%s: %s\n", "INFO", fmt);
    va_start(args, fmt);
    vprintf(temp, args);
    va_end(args);
}

void ocplog_null(const char* fmt, ...){}
