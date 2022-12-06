#include "console.h"
#include <stdio.h>
OCPInfoType  OCPInfo  = printf;
OCPDebugType OCPDebug = printf;

int App() { OCPInfo("Reading input"); }
