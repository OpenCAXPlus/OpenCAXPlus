#include "console.h"

OCPInfoTypePtr  OCPInfo  = printf;
OCPDebugTypePtr OCPDebug = printf;

int App() { OCPInfo("Reading input"); }
