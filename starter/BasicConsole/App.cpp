#include "console.h"

OCPInfo  = ocplog_Info;
OCPDebug = ocplog_Debug;
int App()
{
    OCPInfo("Testing");
    OCPDebug("Testing");
    return 0;
}