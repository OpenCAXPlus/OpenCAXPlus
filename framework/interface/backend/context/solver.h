#ifndef _OCP_FRAMEWORK_CONTEXT_SOLVER_H_
#define _OCP_FRAMEWORK_CONTEXT_SOLVER_H_

#include "interface/backend/data/csr.h"
#include "interface/backend/data/vec.h"

/**
 * @brief The CSR solver context for Ax=b
 */
typedef struct _OCPContextCSRSolver {
    dOCPCSRMatPtr A; // input
    dOCPVecPtr    b; // input
    dOCPVecPtr    x; // output
    // we need to discuss any other configurational variables
} OCPContextCSRSolver;
typedef OCPContextCSRSolver* OCPContextCSRSolverPtr;

#endif