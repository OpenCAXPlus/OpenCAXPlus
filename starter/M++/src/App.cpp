// file:   Main.C
// author: Christian Wieners Jiping Xin

#include "Poisson.h"

#include "App.h"


int App (int argc, char* argv[]) {
	OCPInfo("The M++ starter");
	DPO dpo(&argc,&argv);
	string Model = "test";
	ReadConfig(Settings,"Model",Model);
	if (Model == "Poisson") PoissonMain();

    return 0;
}
