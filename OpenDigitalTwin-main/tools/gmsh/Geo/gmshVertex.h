// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GMSH_VERTEX_H_
#define _GMSH_VERTEX_H_

#include "GVertex.h"

class Vertex;

class gmshVertex : public GVertex {
 protected:
  Vertex *v;

 public:
  gmshVertex(GModel *m, Vertex *_v);
  virtual ~gmshVertex() {}
  virtual void resetMeshAttributes();
  virtual GPoint point() const;
  virtual double x() const;
  virtual double y() const;
  virtual double z() const;
  virtual void setPosition(GPoint &p);
  virtual GeomType geomType() const;
  ModelType getNativeType() const { return GmshModel; }
  void *getNativePtr() const { return v; }
  virtual void setPrescribedMeshSizeAtVertex(double l);
  virtual SPoint2 reparamOnFace(const GFace *gf, int) const;
  virtual void writeGEO(FILE *fp, const std::string &meshSizeParameter="");
  void resetNativePtr(Vertex *_v);
};

#endif
