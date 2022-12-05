#ifndef _OCP_FRAMEWORK_BACKEND_SOLVER_H_
#define _OCP_FRAMEWORK_BACKEND_SOLVER_H_

#include "backend/csr.h"
#include "backend/vec.h"
#include "common/common.h"
/**
 * @brief The CSR solver context for Ax=b
 */
typedef struct _ocpCSRSolverContext {
    ocpName             name;
    ocpCSRMatFloat64Ptr A; // input
    ocpVecFloat64Ptr    b; // input
    ocpVecFloat64Ptr    x; // output
    // we need to discuss any other configurational variables
} ocpCSRSolverContext;
typedef ocpCSRSolverContext* ocpCSRSolverContextPtr;

typedef struct _ocpSolverContext {
    ocpName name;
} ocpSolverContext;
typedef ocpSolverContext* ocpSolverContextPtr;

typedef ocpStatus     OCPCSRSolver(ocpCSRSolverContextPtr a);
typedef OCPCSRSolver* OCPCSRSolverPtr;


ocpStatus OCPSolve(OCPCSRSolverPtr s, ocpCSRSolverContextPtr c){
    return s(c);
}

#endif 