#ifndef _OCP_FRAMEWORK_BACKEND_PARAMETER_H_
#define _OCP_FRAMEWORK_BACKEND_PARAMETER_H_

#include "ocp/common.h"
/**
 * @brief The general purpose parameter struct
 */

typedef struct _ocpParameterType {
    ocpName      name;
    int          n; // number of parameters
    ocpIdPtr       id;
    ocpValuePtr    value;
    ocpDataTypePtr type;
    // we need to discuss any other configurational variables
} ocpParameterType;
typedef ocpParameterType* ocpParameterTypePtr;

// return void pointer, so that you can cast as anything
// typedef void* (*OCPParameterParserType)(ocpParameterTypePtr a);
typedef ocpStatus (*OCPParameterAllocateType)(ocpParameterTypePtr a, int n);
typedef ocpStatus (*OCPParameterDeallocateType)(ocpParameterTypePtr a);

#endif