// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.
//
// CGNSOptions.h - Copyright (C) 2008 S. Guzik, C. Geuzaine, J.-F. Remacle

#ifndef _CGNSOPTIONS_H_
#define _CGNSOPTIONS_H_

#include <string>

class CGNSOptions
{
 public:
  enum CGNSLocationType {
    LocVertex = 0,
    LocFace = 1
  };

  std::string baseName;
  std::string zoneName;
  std::string interfaceName;
  std::string patchName;
  int gridConnectivityLocation;         // Location of connectivity(values
                                        // CGNSLocationType)
  int bocoLocation;                     // Location of BC (values
                                        // CGNSLocationType)
  int normalSource;                     // Source for BC normal data
                                        // 0 - do not write normals
                                        // 1 - geometry
                                        // 2 - elements
  int vectorDim;                        // Number of dimensions in a vector
                                        // (only relevant for a 2D mesh)
  bool writeBC;
  bool writeUserDef;                    // T - write user-defined elements for
                                        //     element types unsupported by CGNS

  CGNSOptions(){ setDefaults(); }
  ~CGNSOptions(){}
  void setDefaults()
  {
    baseName = "Base_1";
    zoneName = "Zone_&I%4&";
    interfaceName = "Interface_&I%4&";
    patchName = "Patch_&I%4&";
    gridConnectivityLocation = 0;
    bocoLocation = 0;
    normalSource = 1;
    vectorDim = 2;
    writeBC = true;
    writeUserDef = false;
  }
};

#endif
