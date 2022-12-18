#ifndef _OCP_TOOLKIT_OCPLOG_H
#define _OCP_TOOLKIT_OCPLOG_H

#include "ocp/backend/log.h"

#ifdef __cplusplus
extern "C" {
#endif

void ocplog_info(const char* fmt, ...);
void ocplog_debug(const char* fmt, ...);
void ocplog_null(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif