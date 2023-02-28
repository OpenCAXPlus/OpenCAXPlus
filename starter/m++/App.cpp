// file:   Main.C
// author: Christian Wieners Jiping Xin

#include "App.h"
#include "Poisson.h"
#include "m++.h"
#include "mpi.h"

#ifdef DCOMPLEX
#    error undef DCOMPLEX in src/Compiler.h
#endif

void PoissonMain();

int app(int argv, char** argc)
{
    DPO    dpo(&argv, &argc);
    string Model = "test";
    ReadConfig(Settings, "Model", Model);
    if (Model == "Poisson") PoissonMain();

    return 0;
}
