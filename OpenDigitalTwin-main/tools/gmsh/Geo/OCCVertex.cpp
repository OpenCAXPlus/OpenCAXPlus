// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include "GmshConfig.h"
#include "GModel.h"
#include "MVertex.h"
#include "MPoint.h"
#include "OCCVertex.h"
#include "OCCEdge.h"
#include "OCCFace.h"

#if defined(HAVE_OCC)

#include "GModelIO_OCC.h"
#include <gp_Pnt.hxx>

OCCVertex::OCCVertex(GModel *m, int num, TopoDS_Vertex v, double lc)
  : GVertex(m, num, lc), _v(v)
{
  max_curvature = -1;
  gp_Pnt pnt = BRep_Tool::Pnt(_v);
  _x = pnt.X();
  _y = pnt.Y();
  _z = pnt.Z();
  if(model()->getOCCInternals())
    model()->getOCCInternals()->bind(_v, num);
}

OCCVertex::~OCCVertex()
{
  if(model()->getOCCInternals())
    model()->getOCCInternals()->unbind(_v, tag());
}

void OCCVertex::setPosition(GPoint &p)
{
  _x = p.x();
  _y = p.y();
  _z = p.z();
  if(mesh_vertices.size()){
    mesh_vertices[0]->x() = p.x();
    mesh_vertices[0]->y() = p.y();
    mesh_vertices[0]->z() = p.z();
  }
}

double max_surf_curvature(const GVertex *gv, double x, double y, double z,
                          const GEdge *_myGEdge)
{
  std::list<GFace *> faces = _myGEdge->faces();
  std::list<GFace *>::iterator it = faces.begin();
  double curv = 1.e-22;
  while(it != faces.end()){
    SPoint2 par = gv->reparamOnFace((*it), 1);
    double cc = (*it)->curvatureDiv(par);
    if(cc > 0) curv = std::max(curv, cc);
    ++it;
  }
  return curv;
}

double OCCVertex::max_curvature_of_surfaces() const
{
  if(max_curvature < 0){
    for(std::list<GEdge*>::const_iterator it = l_edges.begin();
        it != l_edges.end(); ++it){
      max_curvature = std::max(max_surf_curvature(this, x(), y(), z(), *it),
                               max_curvature);
    }
    // printf("max curvature (%d) = %12.5E lc = %12.5E\n",tag(),
    //        max_curvature, prescribedMeshSizeAtVertex());
  }
  return max_curvature;
}

SPoint2 OCCVertex::reparamOnFace(const GFace *gf, int dir) const
{
  std::list<GEdge*>::const_iterator it = l_edges.begin();
  while(it != l_edges.end()){
    std::list<GEdge*> l = gf->edges();
    if(std::find(l.begin(), l.end(), *it) != l.end()){
      if((*it)->isSeam(gf)){
        const TopoDS_Face *s = (TopoDS_Face*)gf->getNativePtr();
        const TopoDS_Edge *c = (TopoDS_Edge*)(*it)->getNativePtr();
        double s1,s0;
        Handle(Geom2d_Curve) curve2d = BRep_Tool::CurveOnSurface(*c, *s, s0, s1);
        if((*it)->getBeginVertex() == this)
          return (*it)->reparamOnFace(gf, s0, dir);
        else if((*it)->getEndVertex() == this)
          return (*it)->reparamOnFace(gf, s1, dir);
      }
    }
    ++it;
  }
  it = l_edges.begin();
  while(it != l_edges.end()){
    std::list<GEdge*> l = gf->edges();
    if(std::find(l.begin(), l.end(), *it) != l.end()){
      if (gf->getNativeType() == GEntity::OpenCascadeModel){
	const TopoDS_Face *s = (TopoDS_Face*)gf->getNativePtr();
	const TopoDS_Edge *c = (TopoDS_Edge*)(*it)->getNativePtr();
	double s1,s0;
	Handle(Geom2d_Curve) curve2d = BRep_Tool::CurveOnSurface(*c, *s, s0, s1);
	if((*it)->getBeginVertex() == this)
	  return (*it)->reparamOnFace(gf, s0, dir);
	else if((*it)->getEndVertex() == this)
	  return (*it)->reparamOnFace(gf, s1, dir);
      }
      //if not OCCFace (OK for planar faces)
      else{
	const GPoint pt = point();
	SPoint3 sp(pt.x(), pt.y(), pt.z());
	gf->parFromPoint(sp);
      }
    }
    ++it;
  }

  // normally never here
  return GVertex::reparamOnFace(gf, dir);
}

#endif
