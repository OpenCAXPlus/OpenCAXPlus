
#include "ocp/backend/log.h"
#include "ocp/backend/solver.h"

#include "fasp.h"
#include "fasp_functs.h"

#ifdef __cplusplus
extern "C" {
#endif

ocpStatus FASP_CSRSolver(ocpCSRSolverContextTypePtr context);

extern OCPInfoType OCPInfo;

#ifdef __cplusplus
}
#endif