#include "console.h"
#include <stdio.h>
OCPInfoType  OCPInfo  = printf;
OCPDebugType OCPDebug = printf;

int App(int _argc, char* _argv[]) { OCPInfo("Reading input"); }
