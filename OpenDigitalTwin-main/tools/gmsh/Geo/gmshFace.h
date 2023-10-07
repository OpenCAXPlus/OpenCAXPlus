// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GMSH_FACE_H_
#define _GMSH_FACE_H_

#include "GFace.h"

class Surface;

class gmshFace : public GFace {
 protected:
  Surface *s;
  bool buildSTLTriangulation(bool force);
 public:
  gmshFace(GModel *m, Surface *face);
  virtual ~gmshFace(){}
  Range<double> parBounds(int i) const;
  void setModelEdges(std::list<GEdge*> &);
  using GFace::point;
  virtual GPoint point(double par1, double par2) const;
  virtual GPoint closestPoint(const SPoint3 &queryPoint,
                              const double initialGuess[2]) const;
  virtual bool containsPoint(const SPoint3 &pt) const;
  virtual double getMetricEigenvalue(const SPoint2 &);
  virtual SVector3 normal(const SPoint2 &param) const;
  virtual Pair<SVector3,SVector3> firstDer(const SPoint2 &param) const;
  virtual void secondDer(const SPoint2 &, SVector3 *, SVector3 *, SVector3 *) const;
  virtual GEntity::GeomType geomType() const;
  ModelType getNativeType() const { return GmshModel; }
  void *getNativePtr() const { return s; }
  virtual SPoint2 parFromPoint(const SPoint3 &, bool onSurface=true) const;
  virtual void resetMeshAttributes();
  void resetNativePtr(Surface *_s);
  bool degenerate(int dim) const;
};

#endif
