#include "backend/log/spdlog/log.h"
#include <stdio.h>

void InputRead()
{
    LoggerSetup();
    INFO("Some info testing");
}


void SimulationSetup() { INFO("User Setup simulation"); }
void SimulationRun() { INFO("User Run simulation"); }
void ResultOutput() { INFO("User Output results"); }
void Cleanup() { INFO("User Cleanup"); }