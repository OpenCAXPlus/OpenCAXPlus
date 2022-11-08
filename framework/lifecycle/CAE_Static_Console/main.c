
#include "main.h"
#include <stdio.h>
int main(int argc, const char* argv[])
{

#ifdef PREEVERYTHING
    PreEverything();
#else
    OCPINFO("CAE Static console application starting");
#endif

    InputRead(); // user defined input parameter

    // SimulationSetup(); // user defined setup simulation

    // SimulationRun(); // user defined run simulation

    // ResultOutput(); // user defined output result

#ifdef POSTEVERYTHING
    PostEverything();
#else
    OCPINFO("CAE Static console application finishing");
#endif

    return 0;
}