#include "console.h"
#include "spdlog/log.h"

OCPInfoTypePtr        OCPInfo        = spdlog_Info;
OCPDebugTypePtr       OCPDebug       = spdlog_Null;
// OCPSetLogLevelTypePtr OCPSetLogLevel = spdlog_set_level;

// OCPInfoTypePtr  OCPInfo  = info;
// OCPDebugTypePtr OCPDebug = debug;
// #define OCPInfo(...)  spdlog::info(__VA_ARGS__)
// #define OCPDebug(...) spdlog::debug(__VA_ARGS__)