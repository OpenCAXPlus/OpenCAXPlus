#ifndef _OCP_FRAMEWORK_BACKEND_SOLVER_H_
#define _OCP_FRAMEWORK_BACKEND_SOLVER_H_

#include "ocp/backend/csr.h"
#include "ocp/backend/parameter.h"
#include "ocp/backend/vec.h"
#include "ocp/common.h"
/**
 * @brief The CSR solver context for Ax=b
 */
typedef struct _ocpCSRSolverContextType {
    ocpName             name;
    ocpCSRMatFloat64Ptr A;     // input
    ocpVecFloat64Ptr    b;     // input
    ocpVecFloat64Ptr    x;     // output
    ocpParameterTypePtr param; // the solver paramerters
    // we need to discuss any other configurational variables
} ocpCSRSolverContextType;
typedef ocpCSRSolverContextType* ocpCSRSolverContextTypePtr;

typedef ocpStatus (*OCPCSRSolverType)(ocpCSRSolverContextTypePtr a);

#endif