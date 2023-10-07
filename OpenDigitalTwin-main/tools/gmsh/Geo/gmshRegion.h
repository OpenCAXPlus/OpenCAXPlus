// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GMSH_REGION_H_
#define _GMSH_REGION_H_

#include "GRegion.h"

class Volume;

class gmshRegion : public GRegion {
 protected:
  ::Volume *v;
 public:
  gmshRegion(GModel *m, ::Volume *_v);
  virtual ~gmshRegion() {}
  virtual GeomType geomType() const;
  ModelType getNativeType() const { return GmshModel; }
  void *getNativePtr() const { return v; }
  virtual void resetMeshAttributes();
  void resetNativePtr(::Volume *_v);
};

#endif
