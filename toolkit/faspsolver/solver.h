
#include "ocp/backend/log.h"
#include "ocp/backend/solver.h"

#include "fasp.h"
#include "fasp_functs.h"

#ifdef __cplusplus
extern "C" {
#endif

ocpStatusEnum fasp_CSR_solver(ocpCSRSolverContextStructPtr context);

extern ocpInfoFunc ocp_info;

#ifdef __cplusplus
}
#endif