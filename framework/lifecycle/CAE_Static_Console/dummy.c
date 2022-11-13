#include "framework/interface/backend/log/log.h"
#include "global.h"

int InputRead() { INFO("Reading input"); }
int SimulationSetup() { INFO("Setup simulation"); }
int SimulationRun() { INFO("Run simulation"); }
int ResultOutput() { INFO("Output results"); }
int Cleanup() { INFO("Cleanup"); }