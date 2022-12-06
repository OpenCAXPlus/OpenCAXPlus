#include "App.h"

int App()
{
    OCPInfo("Testing info");
    OCPDebug("Testing debug");

    ocpCSRSolverContextTypePtr context;
    OCPCSRSolver(context);
    return 0;
}