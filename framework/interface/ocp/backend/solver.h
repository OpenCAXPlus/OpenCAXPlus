#ifndef _OCP_INTERFACE_BACKEND_SOLVER_H_
#define _OCP_INTERFACE_BACKEND_SOLVER_H_

#include "ocp/backend/csr.h"
#include "ocp/backend/parameter.h"
#include "ocp/backend/vec.h"
#include "ocp/common.h"
/**
 * @brief The CSR solver context for Ax=b
 */
typedef struct _ocpCSRSolverContextStruct {
    ocpString32Type               name;
    ocpCSRMatFloat64StructPtr A;     // input
    ocpVecFloat64StructPtr    b;     // input
    ocpVecFloat64StructPtr    x;     // output
    ocpParameterStructPtr     param; // the solver paramerters
    // we need to discuss any other configurational variables
} ocpCSRSolverContextStruct;
typedef ocpCSRSolverContextStruct* ocpCSRSolverContextStructPtr;

typedef ocpStatusEnum (*ocpCSRSolverFunc)(ocpCSRSolverContextStructPtr a);

#endif