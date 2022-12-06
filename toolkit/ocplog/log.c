#include "log.h"
#include <stdarg.h>
#include <stdio.h>

void ocplog_Info(const char* fmt, ...)
{
    va_list args;
    char    temp[512] = {'\0'};
    sprintf(temp, "%s: %s\n", "INFO", fmt);
    va_start(args, fmt);
    vprintf(temp, args);
    va_end(args);
}

void ocplog_Debug(const char* fmt, ...)
{
    va_list args;
    char    temp[512] = {'\0'};
    sprintf(temp, "%s: %s\n", "INFO", fmt);
    va_start(args, fmt);
    vprintf(temp, args);
    va_end(args);
}

void ocplog_Null(const char* fmt, ...){}
