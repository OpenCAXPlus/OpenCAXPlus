#ifndef _OCP_TOOLKIT_SPDLOG_H_
#define _OCP_TOOLKIT_SPDLOG_H_

#include "ocp/backend/log.h"
#include "spdlog/spdlog.h"

void spdlog_Info(const char* fmt, ...);
void spdlog_Debug(const char* fmt, ...);
// void spdlog_set_level(const char* lvl);
void spdlog_Null(const char* fmt, ...);

#endif