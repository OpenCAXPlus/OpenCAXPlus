// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _LAMBDA2_H_
#define _LAMBDA2_H_

#include "Plugin.h"

extern "C"
{
  GMSH_Plugin *GMSH_RegisterLambda2Plugin();
}

class GMSH_Lambda2Plugin : public GMSH_PostPlugin
{
 public:
  GMSH_Lambda2Plugin(){}
  std::string getName() const { return "Lambda2"; }
  std::string getShortHelp() const
  {
    return "Detect vortices";
  }
  std::string getHelp() const;
  std::string getAuthor() const { return "E. Marchandise"; }
  int getNbOptions() const;
  StringXNumber* getOption(int iopt);
  PView *execute(PView *);
};

#endif
