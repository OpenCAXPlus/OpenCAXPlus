// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <string.h>
#include <math.h>
#include "MVertex.h"
#include "GModel.h"
#include "GVertex.h"
#include "GEdge.h"
#include "GFace.h"
#include "GFaceCompound.h"
#include "discreteDiskFace.h"
#include "GmshMessage.h"
#include "StringUtils.h"

double angle3Vertices(const MVertex *p1, const MVertex *p2, const MVertex *p3)
{
  SVector3 a(p1->x() - p2->x(), p1->y() - p2->y(), p1->z() - p2->z());
  SVector3 b(p3->x() - p2->x(), p3->y() - p2->y(), p3->z() - p2->z());
  SVector3 c = crossprod(a, b);
  double sinA = c.norm();
  double cosA = dot(a, b);
  return atan2 (sinA, cosA);
}

MVertex::MVertex(double x, double y, double z, GEntity *ge, int num)
  : _visible(1), _order(1), _x(x), _y(y), _z(z), _ge(ge)
{
#if defined(_OPENMP)
#pragma omp critical
#endif
  {
    // we should make GModel a mandatory argument to the constructor
    GModel *m = GModel::current();
    if(num){
      _num = num;
      m->setMaxVertexNumber(std::max(m->getMaxVertexNumber(), _num));
    }
    else{
      m->setMaxVertexNumber(m->getMaxVertexNumber() + 1);
      _num = m->getMaxVertexNumber();
    }
    _index = num;
  }
}

void MVertex::deleteLast()
{
  GModel *m = GModel::current();
  if(_num == m->getMaxVertexNumber())
    m->setMaxVertexNumber(m->getMaxVertexNumber() - 1);
  delete this;
}

void MVertex::forceNum(int num)
{
#if defined(_OPENMP)
#pragma omp critical
#endif
  {
    _num = num;
    GModel::current()->setMaxVertexNumber
      (std::max(GModel::current()->getMaxVertexNumber(), _num));
  }
}

void MVertex::writeMSH(FILE *fp, bool binary, bool saveParametric, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  if(!binary){
    fprintf(fp, "%d %.16g %.16g %.16g ", _index, x() * scalingFactor,
            y() * scalingFactor, z() * scalingFactor);
  }
  else{
    fwrite(&_index, sizeof(int), 1, fp);
    double data[3] = {x() * scalingFactor, y() * scalingFactor, z() * scalingFactor};
    fwrite(data, sizeof(double), 3, fp);
  }

  int zero = 0;
  if(!onWhat() || !saveParametric){
    if(!binary)
      fprintf(fp, "0\n");
    else
      fwrite(&zero, sizeof(int), 1, fp);
  }
  else{
    int entity = onWhat()->tag();
    int dim = onWhat()->dim();
    if(!binary)
      fprintf(fp, "%d %d ", entity, dim);
    else{
      fwrite(&entity, sizeof(int), 1, fp);
      fwrite(&dim, sizeof(int), 1, fp);
    }
    switch(dim){
    case 0:
      if(!binary)
        fprintf(fp, "\n");
      break;
    case 1:
      {
        double _u;
        getParameter(0, _u);
        if(!binary)
          fprintf(fp, "%.16g\n", _u);
        else
          fwrite(&_u, sizeof(double), 1, fp);
      }
      break;
    case 2:
      {
        double _u, _v;
        getParameter(0, _u);
        getParameter(1, _v);
        if(!binary)
          fprintf(fp, "%.16g %.16g\n", _u, _v);
        else{
          fwrite(&_u, sizeof(double), 1, fp);
          fwrite(&_v, sizeof(double), 1, fp);
        }
      }
      break;
    default:
      if(!binary)
        fprintf(fp, "0 0 0\n");
      else{
        fwrite(&zero, sizeof(int), 1, fp);
        fwrite(&zero, sizeof(int), 1, fp);
        fwrite(&zero, sizeof(int), 1, fp);
      }
      break;
    }
  }
}

void MVertex::writeMSH2(FILE *fp, bool binary, bool saveParametric, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  int myDim = 0, myTag = 0;
  if(saveParametric){
    if(onWhat()){
      myDim = onWhat()->dim();
      myTag = onWhat()->tag();
    }
    else
      saveParametric = false;
  }

  if(!binary){
    if(!saveParametric)
      fprintf(fp, "%d %.16g %.16g %.16g\n", _index, x() * scalingFactor,
              y() * scalingFactor, z() * scalingFactor);
    else
      fprintf(fp, "%d %.16g %.16g %.16g %d %d", _index, x() * scalingFactor,
              y() * scalingFactor, z() * scalingFactor, myDim, myTag);
  }
  else{
    fwrite(&_index, sizeof(int), 1, fp);
    double data[3] = {x() * scalingFactor, y() * scalingFactor, z() * scalingFactor};
    fwrite(data, sizeof(double), 3, fp);
    if(saveParametric){
      fwrite(&myDim, sizeof(int), 1, fp);
      fwrite(&myTag, sizeof(int), 1, fp);
    }
  }

  if(saveParametric){
    if(myDim == 1){
      double _u;
      getParameter(0, _u);
      if(!binary)
        fprintf(fp, " %.16g\n", _u);
      else
        fwrite(&_u, sizeof(double), 1, fp);
    }
    else if (myDim == 2){
      double _u, _v;
      getParameter(0, _u);
      getParameter(1, _v);
      if(!binary)
        fprintf(fp, " %.16g %.16g\n", _u, _v);
      else{
        fwrite(&_u, sizeof(double), 1, fp);
        fwrite(&_v, sizeof(double), 1, fp);
      }
    }
    else
      if(!binary)
        fprintf(fp, "\n");
  }
}

void MVertex::writePLY2(FILE *fp)
{
  if(_index < 0) return; // negative index vertices are never saved

  fprintf(fp, "%.16g %.16g %.16g\n", x(), y(), z());
}

void MVertex::writeVRML(FILE *fp, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  fprintf(fp, "%.16g %.16g %.16g,\n",
          x() * scalingFactor, y() * scalingFactor, z() * scalingFactor);
}

void MVertex::writeUNV(FILE *fp, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  int coord_sys = 1;
  int displacement_coord_sys = 1;
  int color = 11;
  fprintf(fp, "%10d%10d%10d%10d\n", _index, coord_sys, displacement_coord_sys, color);
  // hack to print the numbers with "D+XX" exponents
  char tmp[128];
  sprintf(tmp, "%25.16E%25.16E%25.16E\n", x() * scalingFactor,
          y() * scalingFactor, z() * scalingFactor);
  for(unsigned int i = 0; i < strlen(tmp); i++) if(tmp[i] == 'E') tmp[i] = 'D';
  fprintf(fp, "%s", tmp);
}

void MVertex::writeVTK(FILE *fp, bool binary, double scalingFactor, bool bigEndian)
{
  if(_index < 0) return; // negative index vertices are never saved

  if(binary){
    double data[3] = {x() * scalingFactor, y() * scalingFactor, z() * scalingFactor};
    // VTK always expects big endian binary data
    if(!bigEndian) SwapBytes((char*)data, sizeof(double), 3);
    fwrite(data, sizeof(double), 3, fp);
  }
  else{
    fprintf(fp, "%.16g %.16g %.16g\n",
            x() * scalingFactor, y() * scalingFactor, z() * scalingFactor);
  }
}

void MVertex::writeTOCHNOG(FILE *fp, int dim, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved
  if(dim == 2) {
    fprintf(fp, "node %d %.16g %.16g\n", _index,
            x() * scalingFactor, y() * scalingFactor);
  }
  else if (dim == 3) {
    fprintf(fp, "node %d %.16g %.16g %.16g\n", _index,
            x() * scalingFactor, y() * scalingFactor, z() * scalingFactor);
  }
  else if (dim == 1) {
    fprintf(fp, "node %d %.16g\n", _index, x() * scalingFactor);
  }
  else {
    fprintf(fp, "ERROR -- unsupported dimension: %d\n", dim);
  }
}

void MVertex::writeMESH(FILE *fp, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  fprintf(fp, " %20.14G      %20.14G      %20.14G      %d\n",
          x() * scalingFactor, y() * scalingFactor, z() * scalingFactor,
          _ge ? _ge->tag() : 0);
}

void MVertex::writeNEU(FILE *fp, int dim, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  switch(dim) {
  case 3:
    fprintf(fp, "%10d%20.11e%20.11e%20.11e\n",
            _index, x() * scalingFactor, y() * scalingFactor,
            z() * scalingFactor);
    break;
  case 2:
    fprintf(fp, "%10d%20.11e%20.11e\n",
            _index, x() * scalingFactor, y() * scalingFactor);
    break;
  case 1:
    fprintf(fp, "%10d%20.11e\n", _index, x() * scalingFactor);
    break;
  }
}

static void double_to_char8(double val, char *str)
{
  if(val >= 1.e6)
    sprintf(str, "%.2E", val);
  else if(val >= 1.e-3)
    sprintf(str, "%f", val);
  else if(val >= 0)
    sprintf(str, "%.2E", val);
  else if(val >= -1.e-3)
    sprintf(str, "%.1E", val);
  else if(val >= -1.e6)
    sprintf(str, "%f", val);
  else
    sprintf(str, "%.1E", val);

#if defined(WIN32)
  // Windows uses 3 digits in the exponent (which apparently does not
  // conform with ANSI C): remove the extra 0
  if(strlen(str) == 9 && (str[4] == 'E' || str[5] == 'E')){
    str[6] = str[7];
    str[7] = str[8];
  }
#endif

  str[8] = '\0';
}

void MVertex::writeBDF(FILE *fp, int format, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  char xs[17], ys[17], zs[17];
  double x1 = x() * scalingFactor;
  double y1 = y() * scalingFactor;
  double z1 = z() * scalingFactor;
  if(format == 0){
    // free field format (max 8 char per field, comma separated, 10 per line)
    double_to_char8(x1, xs); double_to_char8(y1, ys); double_to_char8(z1, zs);
    fprintf(fp, "GRID,%d,%d,%s,%s,%s\n", _index, 0, xs, ys, zs);
  }
  else if(format == 1){
    // small field format (8 char par field, 10 per line)
    double_to_char8(x1, xs); double_to_char8(y1, ys); double_to_char8(z1, zs);
    fprintf(fp, "GRID    %-8d%-8d%-8s%-8s%-8s\n", _index, 0, xs, ys, zs);
  }
  else{
    // large field format (8 char first/last field, 16 char middle, 6 per line)
    fprintf(fp, "GRID*   %-16d%-16d%-16.9G%-16.9G*N%-6d\n", _index, 0, x1, y1, _index);
    fprintf(fp, "*N%-6d%-16.9G\n", _index, z1);
  }
}

void MVertex::writeINP(FILE *fp, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  fprintf(fp, "%d, %.14g, %.14g, %.14g\n", _index, x() * scalingFactor,
          y() * scalingFactor, z() * scalingFactor);
}

void MVertex::writeDIFF(FILE *fp, bool binary, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  fprintf(fp, " %d ( %25.16E , %25.16E , %25.16E )",
          _index, x() * scalingFactor, y() * scalingFactor, z() * scalingFactor);
}

void MVertex::writeSU2(FILE *fp, int dim, double scalingFactor)
{
  if(_index < 0) return; // negative index vertices are never saved

  if(dim == 2)
    fprintf(fp, "%.16g %.16g %d\n", x() * scalingFactor, y() * scalingFactor,
             _index - 1);
  else
    fprintf(fp, "%.16g %.16g %.16g %d\n", x() * scalingFactor, y() * scalingFactor,
            z() * scalingFactor, _index - 1);
}

bool MVertexLessThanNum::operator()(const MVertex *v1, const MVertex *v2) const
{
  if(v1->getNum() < v2->getNum()) return true;
  return false;
}

double MVertexLessThanLexicographic::tolerance = 1.e-6;

double MVertexLessThanLexicographic::getTolerance()
{
  return tolerance;
}

bool MVertexLessThanLexicographic::operator()(const MVertex *v1, const MVertex *v2) const
{
  // you should not use this unless you know what you are doing; to create
  // unique vertices, use MVertexRTree
  if(v1->x() - v2->x() >  tolerance) return true;
  if(v1->x() - v2->x() < -tolerance) return false;
  if(v1->y() - v2->y() >  tolerance) return true;
  if(v1->y() - v2->y() < -tolerance) return false;
  if(v1->z() - v2->z() >  tolerance) return true;
  return false;
}

static void getAllParameters(MVertex *v, GFace *gf, std::vector<SPoint2> &params)
{
  params.clear();

  if (gf->geomType() == GEntity::CompoundSurface ){
    GFaceCompound *gfc = (GFaceCompound*) gf;
    params.push_back(gfc->parFromVertex(v));
    return;
  }

#if defined(HAVE_ANN) && defined(HAVE_SOLVER)
  if (gf->geomType() == GEntity::DiscreteDiskSurface ){
    discreteDiskFace *gfc = (discreteDiskFace*) gf;
    params.push_back(gfc->parFromVertex(v));
    return ;
  }
#endif

  if(v->onWhat()->dim() == 0){
    GVertex *gv = (GVertex*)v->onWhat();
    std::list<GEdge*> ed = gv->edges();
    bool seam = false;
    for(std::list<GEdge*>::iterator it = ed.begin(); it != ed.end(); it++){
      if((*it)->isSeam(gf)) {
        Range<double> range = (*it)->parBounds(0);
        if (gv == (*it)->getBeginVertex()){
          params.push_back((*it)->reparamOnFace(gf, range.low(),-1));
          params.push_back((*it)->reparamOnFace(gf, range.low(), 1));
        }
        if (gv == (*it)->getEndVertex()){
          params.push_back((*it)->reparamOnFace(gf, range.high(),-1));
          params.push_back((*it)->reparamOnFace(gf, range.high(), 1));
        }
        if (gv != (*it)->getBeginVertex() && gv != (*it)->getEndVertex()){
          Msg::Warning("Strange!");
        }
        seam = true;
      }
    }
    if (!seam)
      params.push_back(gv->reparamOnFace(gf, 1));
  }
  else if(v->onWhat()->dim() == 1){
    GEdge *ge = (GEdge*)v->onWhat();
    if(!ge->haveParametrization()) return;
    double UU;
    v->getParameter(0, UU);
    if (UU == 0.0)
      UU = ge->parFromPoint(v->point());
    params.push_back(ge->reparamOnFace(gf, UU, 1));
    if(ge->isSeam(gf))
      params.push_back(ge->reparamOnFace(gf, UU, -1));
  }
  else{
    double UU, VV;
    if(v->onWhat() == gf && v->getParameter(0, UU) && v->getParameter(1, VV))
      params.push_back(SPoint2(UU, VV));
  }
}

bool reparamMeshEdgeOnFace(MVertex *v1, MVertex *v2, GFace *gf,
                           SPoint2 &param1, SPoint2 &param2)
{
  std::vector<SPoint2> p1, p2;
  getAllParameters(v1, gf, p1);
  getAllParameters(v2, gf, p2);
  if (p1.size() == 1 && p2.size() == 1){
    param1 = p1[0];
    param2 = p2[0];
  }
  else if(p1.size() >= 1 && p2.size() >= 1){
    int imin = 0;
    int jmin = 0;
    {
      double d =
	(p2[0].x() - p1[0].x()) * (p2[0].x() - p1[0].x()) +
	(p2[0].y() - p1[0].y()) * (p2[0].y() - p1[0].y());
      for (unsigned int i=0;i<p2.size();i++){
	double d1 =
	  (p2[i].x() - p1[0].x()) * (p2[i].x() - p1[0].x()) +
	  (p2[i].y() - p1[0].y()) * (p2[i].y() - p1[0].y());
	if (d1 < d){
	  imin = i;
	  d = d1;
	}
      }
    }
    {
      double d =
	(p2[0].x() - p1[0].x()) * (p2[0].x() - p1[0].x()) +
	(p2[0].y() - p1[0].y()) * (p2[0].y() - p1[0].y());
      for (unsigned int i=0;i<p1.size();i++){
	double d1 =
	  (p2[0].x() - p1[i].x()) * (p2[0].x() - p1[i].x()) +
	  (p2[0].y() - p1[i].y()) * (p2[0].y() - p1[i].y());
	if (d1 < d){
	  jmin = i;
	  d = d1;
	}
      }
    }
    param1 = p1[jmin];
    param2 = p2[imin];
  }
  else{
    // brute force!
    param1 = gf->parFromPoint(SPoint3(v1->x(), v1->y(), v1->z()));
    param2 = gf->parFromPoint(SPoint3(v2->x(), v2->y(), v2->z()));
  }
  return true;
}

bool reparamMeshVertexOnFace(MVertex *v, const GFace *gf, SPoint2 &param,
                             bool onSurface)
{
  if(gf->geomType() == GEntity::CompoundSurface){
    GFaceCompound *gfc = (GFaceCompound*) gf;
    param = gfc->parFromVertex(v);
    return true;
  }

#if defined(HAVE_ANN) && defined(HAVE_SOLVER)
  if (gf->geomType() == GEntity::DiscreteDiskSurface ){
    discreteDiskFace *gfc = (discreteDiskFace*) gf;
    param = gfc->parFromVertex(v);
    return true;
  }
#endif

  if(v->onWhat()->geomType() == GEntity::DiscreteCurve ||
     v->onWhat()->geomType() == GEntity::BoundaryLayerCurve){
     param = gf->parFromPoint(SPoint3(v->x(), v->y(), v->z()), onSurface);
    return true;
  }

  if(v->onWhat()->dim() == 0){
    GVertex *gv = (GVertex*)v->onWhat();
    // hack for bug in periodic curves
    if (gv->getNativeType() == GEntity::GmshModel && gf->geomType() == GEntity::Plane)
      param = gf->parFromPoint(SPoint3(v->x(), v->y(), v->z()), onSurface);
    else
      param = gv->reparamOnFace(gf, 1);
    // shout, we could be on a seam
    std::list<GEdge*> ed = gv->edges();
    for(std::list<GEdge*>::iterator it = ed.begin(); it != ed.end(); it++)
      if((*it)->isSeam(gf)) return false;
  }
  else if(v->onWhat()->dim() == 1){
    GEdge *ge = (GEdge*)v->onWhat();
    double t;
    v->getParameter(0, t);
    param = ge->reparamOnFace(gf, t, 1);
    if(!v->getParameter(0,t)) {
      Msg::Error("Vertex %p not MEdgeVertex", v);
      return false;
      //param = gf->parFromPoint(SPoint3(v->x(), v->y(), v->z()), onSurface);
    }

    // shout, we are on a seam
    if(ge->isSeam(gf))
      return false;
  }
  else{
    double uu, vv;
    if(v->onWhat() == gf && v->getParameter(0, uu) && v->getParameter(1, vv)){
       param = SPoint2(uu, vv);
    }
    else {
      // brute force!
      param = gf->parFromPoint(SPoint3(v->x(), v->y(), v->z()), onSurface);
    }
  }
  return true;
}

bool reparamMeshVertexOnEdge(MVertex *v, const GEdge *ge, double &param)
{
  param = 1.e6;
  Range<double> bounds = ge->parBounds(0);
  bool ok = true;
  if(ge->getBeginVertex() && ge->getBeginVertex()->mesh_vertices[0] == v)
    param = bounds.low();
  else if(ge->getEndVertex() && ge->getEndVertex()->mesh_vertices[0] == v)
    param = bounds.high();
  else
    ok = v->getParameter(0, param);

  if(!ok || param == 1.e6)
    param = ge->parFromPoint(SPoint3(v->x(), v->y(), v->z()));

  if(param < 1.e6) return true;
  return false;
}
