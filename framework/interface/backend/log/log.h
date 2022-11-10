#include <stdarg.h>
#include <stdio.h>
#ifndef _OCP_FRAMEWORK_LOG_H_
#    define _OCP_FRAMEWORK_LOG_H_

#    ifndef NDEBUG
// Release build
#        define OCPTRACE
#        define OCPDEBUG

#    else
// Debug build
#        define OCPTRACE(...) TRACE(__VA_ARGS__)
#        define OCPDEBUG(...) DEBUG(__VA_ARGS__)

#    endif

#    define OCPINFO(...)  INFO(__VA_ARGS__)
#    define OCPWARN(...)  WARN(__VA_ARGS__)
#    define OCPERROR(...) ERROR(__VA_ARGS__)

// Use our logging in the toolkit, or the user him/herself should define the
// TRACE, DEBUG, INFO, WARN, ERROR macros
#    ifndef TRACE
#        define TRACE(fmt, ...) print("TRACE", fmt, ##__VA_ARGS__)
#    endif

#    ifndef DEBUG
#        define DEBUG(fmt, ...) print("DEBUG", fmt, ##__VA_ARGS__)
#    endif

#    ifndef INFO
#        define INFO(fmt, ...) print(" INFO", fmt, ##__VA_ARGS__)
#    endif

#    ifndef WARN
#        define WARN(fmt, ...) print(" WARN", fmt, ##__VA_ARGS__)
#    endif

#    ifndef ERROR
#        define ERROR(fmt, ...) print("ERROR", fmt, ##__VA_ARGS__)
#    endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

void print(const char* tp, const char* fmt, ...);

#ifdef __cplusplus
}
#endif