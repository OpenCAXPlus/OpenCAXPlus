#include "log.h"

#if !defined(__APPLE__) | !defined(__INTEL_COMPILER)
void LoggerSetup() { spdlog::info("Logger Setup"); }
#else
void LoggerSetup() { INFO("Logger Setup"); }
#endif