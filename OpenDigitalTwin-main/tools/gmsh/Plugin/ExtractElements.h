// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _EXTRACT_ELEMENTS_H_
#define _EXTRACT_ELEMENTS_H_

#include "Plugin.h"

extern "C"
{
  GMSH_Plugin *GMSH_RegisterExtractElementsPlugin();
}

class GMSH_ExtractElementsPlugin : public GMSH_PostPlugin
{
 public:
  GMSH_ExtractElementsPlugin(){}
  std::string getName() const { return "ExtractElements"; }
  std::string getShortHelp() const
  {
    return "Extract elements in given value range";
  }
  std::string getHelp() const;
  int getNbOptions() const;
  StringXNumber* getOption(int iopt);  
  PView *execute(PView *);
};

#endif
