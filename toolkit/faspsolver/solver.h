#include "ocp/backend/solver.h"

#include "fasp.h"
#include "fasp_functs.h"

#ifdef __cplusplus
extern "C" {
#endif

void solve();

// OCPCSRSolverPtr FASP_OCPCSRSolver;

ocpStatus FASP_OCPCSRSolver(ocpCSRSolverContextPtr context);

#ifdef __cplusplus
}
#endif