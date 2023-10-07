// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GMODEL_IO_FOURIER_H_
#define _GMODEL_IO_FOURIER_H_

#include <vector>
#include "GmshConfig.h"
#include "GModel.h"

#if defined(HAVE_FOURIER_MODEL)

#include "FM_Reader.h"

class FM_Internals {
 private:
  std::vector<FM::Reader*> reader;

 public:
  FM_Internals() {}
  ~FM_Internals() {}

  void loadFM();
  void loadFM(const char* filename);
  void makeGFace(FM::Patch* patch, GModel* model);
  void buildGModel(FM::Reader* reater, GModel* model);

  FM::Reader* getReader(int tag) 
  { 
    if(tag < (int)reader.size()) return reader[tag];
  }
  int getSize() { return reader.size(); }
  FM::Reader* current() { return reader[reader.size() - 1]; }
};

#endif

#endif
