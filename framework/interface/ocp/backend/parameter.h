#ifndef _OCP_INTERFACE_BACKEND_PARAMETER_H_
#define _OCP_INTERFACE_BACKEND_PARAMETER_H_

#include "ocp/common.h"
/**
 * @brief The general purpose parameter struct
 */

typedef struct _ocpParameterStruct {
    ocpString32Type name;
    ocpString32Type id;
    ocpString32Type value;
    ocpDataEnum     type;
} ocpParameterStruct;
typedef ocpParameterStruct* ocpParameterStructPtr;

// return void pointer, so that you can cast as anything
// typedef void* (*OCPParameterParserType)(ocpParameterTypePtr a);
typedef ocpStatusEnum (*ocpParameterAllocateFunc)(ocpParameterStructPtr a, int n);
typedef ocpStatusEnum (*ocpParameterDeallocateFunc)(ocpParameterStructPtr a);

#endif