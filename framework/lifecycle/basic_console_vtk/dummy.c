#include "console.h"
#include <stdio.h>
ocpInfoFunc  ocp_info  = printf;
ocpDebugFunc ocp_debug = printf;

int app(int _argc, char* _argv[]) { ocp_info("Reading input"); }
