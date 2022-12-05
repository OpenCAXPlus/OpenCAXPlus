// file:   Main.C
// author: Christian Wieners Jiping Xin

#include "Poisson.h"
#include "m++.h"
#include "mpi.h"

// #ifdef DCOMPLEX
// #error undef DCOMPLEX in src/Compiler.h
// #endif

// void PoissonMain ();

// int main (int argv, char** argc) {
// 	DPO dpo(&argv,&argc);
// 	string Model = "test";
// 	ReadConfig(Settings,"Model",Model);
// 	if (Model == "Poisson") PoissonMain();

//     return 0;
// }

int InputRead()
{
    DPO dpo(&argc, &argv);
	string Model = "test";
    ReadConfig(Settings, "Model", Model);
}

int SimulationSetup() {}

int SimulationRun() { PoissonMain(); }

int ResultOutput() {}

int Cleanup() {}