#include "app.h"

ocpInfoFunc  ocp_info  = ocplog_info;
ocpDebugFunc ocp_debug = ocplog_null;

ocpCSRSolverFunc ocp_CSR_solver = fasp_CSR_solver;