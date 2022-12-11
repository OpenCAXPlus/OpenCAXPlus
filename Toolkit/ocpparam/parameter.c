#include "parameter.h"

ocpStatus ocplog_ParameterAllocate(ocpParameterTypePtr a, int n)
{
    a        = calloc(1, sizeof(ocpParameterType));
    a->n     = n;
    a->id    = (ocpId*)calloc(n, sizeof(ocpId));
    a->value = (ocpValue*)calloc(n + 1, sizeof(ocpValue));
    a->type  = (ocpDataType*)calloc(n, sizeof(ocpDataType));
    return ocpOK;
}

ocpStatus ocplog_ParameterDeallocate(ocpParameterTypePtr a)
{
    free(a->id);
    free(a->value);
    free(a->type);
    free(a);
    return ocpOK;
}

ocpValuePtr
ocplog_ParameterGet(ocpParameterTypePtr param, const char* name, ocpDataType type)
{
    for (size_t i = 0; i < param->n; i++) {
        if (strcmp(param->id[i], name) && param->type[i] == ocpInt) {
            return param->value[i];
        }
    }
    return param->value[param->n + 1];
}

int ocplog_ParameterGetInt(ocpParameterTypePtr param,
                           const char*         name,
                           int                 default_value)
{
    int         value = 0;
    ocpValuePtr val   = ocplog_ParameterGet(param, name, ocpInt);
    int         ret   = sscanf(*val, "%d", &value);
    if (ret == 1) {
        return value;
    } else {
        return default_value;
    }
}

float ocplog_ParameterGetFloat32(ocpParameterTypePtr param,
                                 const char*         name,
                                 float               default_value)
{
    float       value = 0;
    ocpValuePtr val   = ocplog_ParameterGet(param, name, ocpFloat32);
    int         ret   = sscanf(*val, "%f", &value);
    if (ret == 1) {
        return value;
    } else {
        return default_value;
    }
}
