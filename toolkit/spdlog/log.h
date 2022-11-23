#ifndef _OCP_TOOLKIT_LOG_H_
#define _OCP_TOOLKIT_LOG_H_
#if !defined(__APPLE__) | !defined(__INTEL_COMPILER)
#    include "spdlog/spdlog.h"
#    ifndef NDEBUG
// Release build
#        define TRACE
#        define DEBUG
#    else
// Debug build
#        define TRACE(...) spdlog::trace(__VA_ARGS__)
#        define DEBUG(...) spdlog::debug(__VA_ARGS__)
#    endif

#    define INFO(...)  spdlog::info(__VA_ARGS__)
#    define WARN(...)  spdlog::warn(__VA_ARGS__)
#    define ERRRO(...) spdlog::error(__VA_ARGS__)
#else
#    include <stdio.h>
#endif
void LoggerSetup();

#endif