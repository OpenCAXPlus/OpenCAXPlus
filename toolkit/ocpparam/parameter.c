#include "parameter.h"

ocpStatusEnum ocplog_parameter_allocate(ocpParameterStructPtr a, int n)
{
    a = calloc(n, sizeof(ocpParameterStruct));
    return OCP_OK;
}

ocpStatusEnum ocplog_parameter_deallocate(ocpParameterStructPtr a)
{
    free(a);
    return OCP_OK;
}
