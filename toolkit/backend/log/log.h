#ifndef _OCP_TOOLKIT_LOG_H_
#define _OCP_TOOLKIT_LOG_H_
#include "spdlog/spdlog.h"

// #ifdef TRACE
// #    undefine TRACE
// #endif
// #ifdef DEBUG
// #    undefine DEBUG
// #endif
// #ifdef INFO
// #    undefine INFO
// #endif
// #ifdef WARN
// #    undefine WARN
// #endif
// #ifdef ERRRO
// #    undefine ERRRO
// #endif
#ifndef NDEBUG
// Release build
#    define TRACE
#    define DEBUG
#else
// Debug build
#    define TRACE(...) spdlog::trace(__VA_ARGS__)
#    define DEBUG(...) spdlog::debug(__VA_ARGS__)
#endif

#define INFO(...)  spdlog::info(__VA_ARGS__)
#define WARN(...)  spdlog::warn(__VA_ARGS__)
#define ERRRO(...) spdlog::error(__VA_ARGS__)

void LoggerSetup();
#endif