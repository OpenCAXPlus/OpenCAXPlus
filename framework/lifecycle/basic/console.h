#include "ocp/backend/log.h"

#ifdef __cplusplus
extern "C" {
#endif

// extern int          argc;
// extern char**       argv;
extern ocpInfoFunc  ocp_info;
extern ocpDebugFunc ocp_debug;
extern int          app(int _argc, char* _argv[]);

#ifdef __cplusplus
}
#endif
