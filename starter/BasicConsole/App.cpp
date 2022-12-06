#include "console.h"
#include <stdio.h>
int App()
{
    // OCPSetLogLevel("Debug");
    printf("%s", "What\n");
    OCPInfo("Testing info");
    OCPDebug("Testing debug");
    return 0;
}