#include "ocp/backend/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int                   argc;
extern char**                argv;
extern OCPInfoTypePtr        OCPInfo;
extern OCPDebugTypePtr       OCPDebug;
// extern OCPSetLogLevelTypePtr OCPSetLogLevel;
extern int                   App();
// extern void                  OCPLog_Null(const char* fmt, ...);

#ifdef __cplusplus
}
#endif
