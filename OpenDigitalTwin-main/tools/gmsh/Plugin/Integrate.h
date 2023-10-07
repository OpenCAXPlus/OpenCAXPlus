// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _INTEGRATE_H_
#define _INTEGRATE_H_

#include "Plugin.h"

extern "C"
{
  GMSH_Plugin *GMSH_RegisterIntegratePlugin();
}

class GMSH_IntegratePlugin : public GMSH_PostPlugin
{
 public:
  GMSH_IntegratePlugin(){}
  std::string getName() const { return "Integrate"; }
  std::string getShortHelp() const
  {
    return "Integrate data";
  }
  std::string getHelp() const;
  int getNbOptions() const;
  StringXNumber* getOption(int iopt);  
  PView *execute(PView *);
};

#endif
