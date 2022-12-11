#ifndef _OCP_TOOLKIT_OCPLOG_H
#define _OCP_TOOLKIT_OCPLOG_H

#include "ocp/backend/log.h"

#ifdef __cplusplus
extern "C" {
#endif

void ocplog_Info(const char* fmt, ...);
void ocplog_Debug(const char* fmt, ...);
void ocplog_Null(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif