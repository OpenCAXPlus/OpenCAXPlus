#include "framework/interface/backend/log/log.h"
#include "toolkit/backend/log/log.h"
#include <stdio.h>

void InputRead()
{
    LoggerSetup();
    INFO("Some info testing");
}