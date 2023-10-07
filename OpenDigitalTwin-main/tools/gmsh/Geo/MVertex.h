// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _MVERTEX_H_
#define _MVERTEX_H_

#include <stdio.h>
#include <set>
#include <map>
#include "SPoint2.h"
#include "SPoint3.h"
#include "MVertexBoundaryLayerData.h"

class GEntity;
class GEdge;
class GFace;
class MVertex;

// A mesh vertex.
class MVertex{
 protected:
  // the immutable id number of the vertex (this number is unique and
  // is guaranteed never to change once a vertex has been created)
  int _num;
  // a vertex index, used for example during mesh generation or when
  // saving a mesh (this index is not necessarily unique, can change
  // after mesh renumbering, etc.). By convention, vertices with
  // negative indices are not saved
  int _index;
  // a visibility and polynomial order flags
  char _visible, _order;
  // the cartesian coordinates of the vertex
  double _x, _y, _z;
  // the geometrical entity the vertex is associated with
  GEntity *_ge;

 public:
  MVertex(double x, double y, double z, GEntity *ge=0, int num=0);
  virtual ~MVertex(){}
  void deleteLast();

  // get/set the visibility flag
  virtual char getVisibility(){ return _visible; }
  virtual void setVisibility(char val){ _visible = val; }

  // get the "polynomial order" of the vertex
  inline int getPolynomialOrder(){ return _order; }
  inline void setPolynomialOrder(int order){ _order = (char)order; }

  // get/set the coordinates
  inline double x() const { return _x; }
  inline double y() const { return _y; }
  inline double z() const { return _z; }
  inline double & x() { return _x; }
  inline double & y() { return _y; }
  inline double & z() { return _z; }

  inline SPoint3 point() const { return SPoint3(_x, _y, _z); }
  inline void setXYZ(double x, double y, double z) { _x = x; _y = y; _z = z; }

  // get/set the parent entity
  inline GEntity* onWhat() const { return _ge; }
  inline void setEntity(GEntity *ge) { _ge = ge; }

  // get the immutab vertex number
  inline int getNum() const { return _num; }

  // force the immutable number (this should normally never be used)
  void forceNum(int num);

  // get/set the index
  inline int getIndex() const { return _index; }
  inline void setIndex(int index) { _index = index; }


  // get/set ith parameter
  virtual bool getParameter(int i, double &par) const { par = 0.; return false; }
  virtual bool setParameter(int i, double par){ return false; }

  // measure distance to another vertex
  double distance(MVertex *v)
  {
    double dx = _x - v->x();
    double dy = _y - v->y();
    double dz = _z - v->z();
    return sqrt(dx * dx + dy * dy + dz * dz);
  }

  // IO routines
  void writeMSH(FILE *fp, bool binary=false, bool saveParametric=false,
                double scalingFactor=1.0);
  void writeMSH2(FILE *fp, bool binary=false, bool saveParametric=false,
                 double scalingFactor=1.0);
  void writePLY2(FILE *fp);
  void writeVRML(FILE *fp, double scalingFactor=1.0);
  void writeUNV(FILE *fp, double scalingFactor=1.0);
  void writeVTK(FILE *fp, bool binary=false, double scalingFactor=1.0,
                bool bigEndian=false);
  void writeTOCHNOG(FILE *fp, int dim, double scalingFactor=1.0);
  void writeMESH(FILE *fp, double scalingFactor=1.0);
  void writeNEU(FILE *fp, int dim, double scalingFactor=1.0);
  void writeBDF(FILE *fp, int format=0, double scalingFactor=1.0);
  void writeINP(FILE *fp, double scalingFactor=1.0);
  void writeDIFF(FILE *fp, bool binary, double scalingFactor=1.0);
  void writeSU2(FILE *fp, int dim, double scalingFactor=1.0);
};

class MEdgeVertex : public MVertex{
 protected:
  double _u, _lc;
 public:
  MVertexBoundaryLayerData* bl_data;

  MEdgeVertex(double x, double y, double z, GEntity *ge, double u, double lc = -1.0,
              int num = 0)
    : MVertex(x, y, z, ge,num), _u(u), _lc(lc), bl_data(0)
  {
  }
  virtual ~MEdgeVertex(){ if(bl_data) delete bl_data; }
  virtual bool getParameter(int i, double &par) const { par = _u; return true; }
  virtual bool setParameter(int i, double par){ _u = par; return true; }
  double getLc() const { return _lc; }
};

class MFaceVertex : public MVertex{
 protected:
  double _u, _v;
 public :
  MVertexBoundaryLayerData* bl_data;

  MFaceVertex(double x, double y, double z, GEntity *ge, double u, double v, int num = 0)
    : MVertex(x, y, z, ge, num), _u(u), _v(v), bl_data(0)
  {
  }
  virtual ~MFaceVertex(){ if(bl_data) delete bl_data; }
  virtual bool getParameter(int i, double &par) const { par = (i ? _v : _u); return true; }
  virtual bool setParameter(int i, double par)
  {
    if(!i)
      _u = par;
    else
      _v = par;
    return true;
  }
};

class MVertexLessThanLexicographic{
  static double tolerance;
public:
  static double getTolerance();
  bool operator()(const MVertex *v1, const MVertex *v2) const;
};

class MVertexLessThanNum{
 public:
  bool operator()(const MVertex *v1, const MVertex *v2) const;
};

bool reparamMeshEdgeOnFace(MVertex *v1, MVertex *v2, GFace *gf,
                           SPoint2 &param1, SPoint2 &param2);
bool reparamMeshVertexOnFace(MVertex *v, const GFace *gf, SPoint2 &param,
                             bool onSurface=true);
bool reparamMeshVertexOnEdge(MVertex *v, const GEdge *ge, double &param);

double angle3Vertices(const MVertex *p1, const MVertex *p2, const MVertex *p3);

inline double distance (MVertex *v1, MVertex *v2)
{
  const double dx = v1->x() - v2->x();
  const double dy = v1->y() - v2->y();
  const double dz = v1->z() - v2->z();
  return sqrt(dx*dx+dy*dy+dz*dz);
}

#endif
