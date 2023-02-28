#ifndef _OCP_TOOLKIT_OCPPARAMETER_H
#define _OCP_TOOLKIT_OCPPARAMETER_H

#include "ocp/backend/parameter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ocpStatusEnum ocplog_parameter_allocate(ocpParameterStructPtr a, int n);
ocpStatusEnum ocplog_parameter_deallocate(ocpParameterStructPtr a);

#ifdef __cplusplus
}
#endif

#endif