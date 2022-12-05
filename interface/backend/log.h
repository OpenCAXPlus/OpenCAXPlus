#include <stdio.h>

#ifndef _OCP_FRAMEWORK_BACKEND_LOG_H_
#    define _OCP_FRAMEWORK_BACKEND_LOG_H_

// #    ifndef NDEBUG
// // Release build
// #        define OCPTRACE
// #        define OCPDEBUG

// #    else
// // Debug build
// #        define OCPTRACE(...) TRACE(__VA_ARGS__)
// #        define OCPDEBUG(...) DEBUG(__VA_ARGS__)

// #    endif

// #    define OCPINFO(...)  INFO(__VA_ARGS__)
// #    define OCPWARN(...)  WARN(__VA_ARGS__)
// #    define OCPERROR(...) ERROR(__VA_ARGS__)

// // Use our logging in the toolkit, or the user him/herself should define the
// // TRACE, DEBUG, INFO, WARN, ERROR macros
// #    ifndef TRACE
// #        define TRACE(fmt, ...) print("TRACE", fmt, ##__VA_ARGS__)
// #    endif

// #    ifndef DEBUG
// #        define DEBUG(fmt, ...) print("DEBUG", fmt, ##__VA_ARGS__)
// #    endif

// #    ifndef INFO
// #        define INFO(fmt, ...) print(" INFO", fmt, ##__VA_ARGS__)
// #    endif

// #    ifndef WARN
// #        define WARN(fmt, ...) print(" WARN", fmt, ##__VA_ARGS__)
// #    endif

// #    ifndef ERROR
// #        define ERROR(fmt, ...) print("ERROR", fmt, ##__VA_ARGS__)
// #    endif

// declare the OCPInfo function interface
typedef void         OCPInfoType(const char* fmt, ...);
typedef OCPInfoType* OCPInfoTypePtr;

// declare the OCPDebug function interface
typedef void          OCPDebugType(const char* fmt, ...);
typedef OCPDebugType* OCPDebugTypePtr;

#endif

// #ifdef __cplusplus
// extern "C" {
// #endif

// void print(const char* tp, const char* fmt, ...)
// {
//     va_list args;
//     char    temp[512] = {'\0'};
//     sprintf(temp, "%s: %s\n", tp, fmt);
//     va_start(args, fmt);
//     vprintf(temp, args);
//     va_end(args);
// }

// #ifdef __cplusplus
// }
// #endif