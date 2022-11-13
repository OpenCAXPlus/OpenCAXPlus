
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int    argc;
char** argv;

int main(int _argc, const char* _argv[])
{
    int status = 0;

    argc = _argc;
    argv = malloc((_argc + 1) * sizeof *argv);
    for (int i = 0; i < argc; ++i) {
        size_t length = strlen(_argv[i]) + 1;
        argv[i]       = malloc(length);
        memcpy(argv[i], _argv[i], length);
    }
    argv[argc] = NULL;
    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

#ifdef PREEVERYTHING
    PreEverything();
#else
    // INFO("CAE Static console application starting");
#endif

    status = InputRead(); // user defined input parameter

    status = SimulationSetup(); // user defined setup simulation

    status = SimulationRun(); // user defined run simulation

    status = ResultOutput(); // user defined output result

    status = Cleanup(); //

#ifdef POSTEVERYTHING
    PostEverything();
#else
    // INFO("CAE Static console application finishing");
#endif

    free(argv);
    return status;
}