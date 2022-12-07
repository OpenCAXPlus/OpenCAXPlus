#ifndef _OCP_TOOLKIT_OCPPARAMETER_H
#define _OCP_TOOLKIT_OCPPARAMETER_H

#include "ocp/backend/parameter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ocpStatus ocplog_ParameterAllocate(ocpParameterTypePtr a, int n);
int       ocplog_ParameterGetInt(ocpParameterTypePtr a, const char* name, int);
float     ocplog_ParameterGetFloat32(ocpParameterTypePtr a, const char* name, float);
ocpStatus ocplog_ParameterDeallocate(ocpParameterTypePtr a);
ocpValuePtr
ocplog_ParameterGet(ocpParameterTypePtr a, const char* name, ocpDataType type);

#ifdef __cplusplus
}
#endif

#endif