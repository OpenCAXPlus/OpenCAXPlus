// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _PROBE_H_
#define _PROBE_H_

#include "Plugin.h"

extern "C"
{
  GMSH_Plugin *GMSH_RegisterProbePlugin();
}

class GMSH_ProbePlugin : public GMSH_PostPlugin
{
  double levelset(double x, double y, double z, double val) const;
  static double callback(int num, int action, double value, double *opt);
  static int iview;
 public:
  GMSH_ProbePlugin(){}
  std::string getName() const { return "Probe"; }
  std::string getShortHelp() const
  {
    return "Get value at point (X, Y, Z)";
  }
  std::string getHelp() const;
  int getNbOptions() const;
  StringXNumber *getOption(int iopt);  
  PView *execute(PView *);

  static double callbackX(int, int, double);
  static double callbackY(int, int, double);
  static double callbackZ(int, int, double);
  static void draw(void *context);
};

#endif
