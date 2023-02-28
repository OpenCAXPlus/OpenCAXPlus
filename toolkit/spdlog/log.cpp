#include "log.h"
#include <stdarg.h>
#include <stdio.h>

// #define SPDLOGINFO(...)  spdlog::info(__VA_ARGS__)
// #define SPDLOGDEBUG(...) spdlog::debug(__VA_ARGS__)

// void spdlog_set_level(const char* lvl)
// {
//     if (strcmp(lvl, "Debug")) {
//         spdlog::set_level(spdlog::level::debug);
//     } else if (strcmp(lvl, "Info")) {
//         spdlog::set_level(spdlog::level::debug);
//     }
// }

void spdlog_info(const char* fmt, ...)
{
    char formatted[1024] = {'\0'};

    va_list args;
    va_start(args, fmt);
    vsnprintf(formatted, 1024, fmt, args);
    va_end(args);
    spdlog::info("{}", formatted);
}

void spdlog_null(const char* fmt, ...) {}

void spdlog_debug(const char* fmt, ...)
{
    char    formatted[1024] = {'\0'};
    va_list args;
    va_start(args, fmt);
    vsnprintf(formatted, 1024, fmt, args);
    va_end(args);
    spdlog::debug("{}", formatted);
}