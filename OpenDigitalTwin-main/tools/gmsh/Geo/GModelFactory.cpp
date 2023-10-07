// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <stdlib.h>
#include "GModelFactory.h"
#include "ListUtils.h"
#include "Context.h"
#include "GVertex.h"
#include "gmshVertex.h"
#include "gmshEdge.h"
#include "gmshFace.h"
#include "gmshRegion.h"
#include "GEdgeCompound.h"
#include "MLine.h"
#include "GModel.h"
#include "Numeric.h"
#include "ExtrudeParams.h"
#include "GModelIO_GEO.h"
#include "Geo.h"
#include "Parser.h"
#include "GmshDefines.h"

GVertex *GeoFactory::addVertex(GModel *gm, double x, double y, double z, double lc)
{
  int num =  gm->getMaxElementaryNumber(0) + 1;

  x *= CTX::instance()->geom.scalingFactor;
  y *= CTX::instance()->geom.scalingFactor;
  z *= CTX::instance()->geom.scalingFactor;
  lc *= CTX::instance()->geom.scalingFactor;
  if(lc == 0.) lc = MAX_LC; // no mesh size given at the point
  Vertex *p;
  p = CreateVertex(num, x, y, z, lc, 1.0);
  Tree_Add(gm->getGEOInternals()->Points, &p);
  p->Typ = MSH_POINT;
  p->Num = num;

  GVertex *v = new gmshVertex(gm, p);
  gm->add(v);

  return v;
}

GEdge *GeoFactory::addLine(GModel *gm, GVertex *start, GVertex *end)
{
  int num =  gm->getMaxElementaryNumber(1) + 1;
  List_T *iList = List_Create(2, 2, sizeof(int));
  int tagBeg = start->tag();
  int tagEnd = end->tag();
  List_Add(iList, &tagBeg);
  List_Add(iList, &tagEnd);

  Curve *c = CreateCurve(num, MSH_SEGM_LINE, 1, iList, NULL,
        -1, -1, 0., 1.);
  Tree_Add(gm->getGEOInternals()->Curves, &c);
  CreateReversedCurve(c);
  List_Delete(iList);
  c->Typ = MSH_SEGM_LINE;
  c->Num = num;

  GEdge *e = new gmshEdge(gm, c, start, end);
  gm->add(e);

  return e;
}

GFace *GeoFactory::addPlanarFace(GModel *gm, std::vector< std::vector<GEdge *> > edges)
{
  std::vector<std::vector<GEdgeSigned> > orientedEdges;
  orientedEdges.reserve(edges.size());
  for (size_t i=0; i< edges.size(); i++){
    std::vector<GEdge *> loop = edges[i];
    orientedEdges.push_back(std::vector<GEdgeSigned>());
    std::vector<GEdgeSigned> &orientedLoop = orientedEdges.back();
    orientedLoop.reserve(loop.size());
    for (size_t j=0; j< loop.size(); j++){
      GEdge *edge = loop[j];
      orientedLoop.push_back(GEdgeSigned(1, edge));
    }
  }

  return _addPlanarFace(gm, orientedEdges, true);
}

GFace *GeoFactory::addPlanarFace(GModel *gm, const std::vector<std::vector<GEdgeSigned> > &edges)
{
  return _addPlanarFace(gm, edges, false);
}

GRegion* GeoFactory::addVolume (GModel *gm, std::vector<std::vector<GFace *> > faces)
{
  //create surface loop
  int nLoops = faces.size();
  std::vector<SurfaceLoop *> vecLoops;
  for (int i=0; i< nLoops; i++){
    int nl=(int)faces[i].size();
    List_T *temp = List_Create(nl, nl, sizeof(int));
    for(int j = 0; j < nl; j++){
      int numFace = faces[i][j]->tag();
      List_Add(temp, &numFace);
    }
    int numfl = gm->getMaxElementaryNumber(2) + 1;
    while (FindSurfaceLoop(numfl)){
      numfl++;
      if (!FindSurfaceLoop(numfl)) break;
    }
    SurfaceLoop *l = CreateSurfaceLoop(numfl, temp);
    vecLoops.push_back(l);
    Tree_Add(gm->getGEOInternals()->SurfaceLoops, &l);
    List_Delete(temp);
  }

  //create volume
  int numv = gm->getMaxElementaryNumber(3) + 1;
  Volume *v = CreateVolume(numv, MSH_VOLUME);
  List_T *temp = List_Create(nLoops, nLoops, sizeof(int));
  for (unsigned int i = 0; i < vecLoops.size(); i++){
    int numl = vecLoops[i]->Num;
    List_Add(temp, &numl);
  }
  SetVolumeSurfaces(v, temp);
  List_Delete(temp);
  Tree_Add(gm->getGEOInternals()->Volumes, &v);
  v->Typ = MSH_VOLUME;
  v->Num = numv;
  v->Recombine3D = 0;

  //gmsh volume
  GRegion *gr = new gmshRegion(gm,v);
  gm->add(gr);

  return gr;
}

GEdge* GeoFactory::addCircleArc(GModel *gm,GVertex *begin, GVertex *center, GVertex *end)
{
  int num =  gm->getMaxElementaryNumber(1) + 1;
  List_T *iList = List_Create(2, 2, sizeof(int));
  int tagBeg = begin->tag();
  int tagMiddle = center->tag();
  int tagEnd = end->tag();
  List_Add(iList, &tagBeg);
  List_Add(iList, &tagMiddle);
  List_Add(iList, &tagEnd);

  Curve *c = CreateCurve(num, MSH_SEGM_CIRC, 1, iList, NULL,
        -1, -1, 0., 1.);
  Tree_Add(gm->getGEOInternals()->Curves, &c);
  CreateReversedCurve(c);
  List_Delete(iList);
  c->Typ = MSH_SEGM_CIRC;
  c->Num = num;

  GEdge *e = new gmshEdge(gm, c, begin, end);
  gm->add(e);

  return e;
}

std::vector<GFace *> GeoFactory::addRuledFaces(GModel *gm,
                                               std::vector<std::vector<GEdge *> > edges)
{
  //create line loops
  std::vector<EdgeLoop *> vecLoops;
  int nLoops = edges.size();
  for (int i=0; i< nLoops; i++){
    int numl = gm->getMaxElementaryNumber(1) + i;
    while (FindEdgeLoop(numl)){
      numl++;
      if (!FindEdgeLoop(numl)) break;
    }
    int nl=(int)edges[i].size();
    List_T *temp = List_Create(nl, nl, sizeof(int));
    for(int j = 0; j < nl; j++){
      int numEdge = edges[i][j]->tag();
      List_Add(temp, &numEdge);
    }
    SortEdgesInLoop(numl, temp);
    EdgeLoop *l = CreateEdgeLoop(numl, temp);
    vecLoops.push_back(l);
    Tree_Add(gm->getGEOInternals()->EdgeLoops, &l);
    l->Num = numl;
    List_Delete(temp);
  }

  //create plane surfaces
  int numf = gm->getMaxElementaryNumber(2) + 1;
  Surface *s = CreateSurface(numf, MSH_SURF_TRIC);
  List_T *iList = List_Create(nLoops, nLoops, sizeof(int));
  for (unsigned int i=0; i< vecLoops.size(); i++){
    int numl = vecLoops[i]->Num;
    List_Add(iList, &numl);
  }
  SetSurfaceGeneratrices(s, iList);
  EndSurface(s);
  Tree_Add(gm->getGEOInternals()->Surfaces, &s);
  s->Typ= MSH_SURF_TRIC;
  s->Num = numf;
  List_Delete(iList);

  //gmsh surface
  GFace *gf = new gmshFace(gm,s);
  gm->add(gf);

  std::vector<GFace*> faces;
  faces.push_back(gf);

  return faces;
}

std::vector<GEntity*> GeoFactory::extrudeBoundaryLayer(GModel *gm, GEntity *e,
                                                       int nbLayers, double hLayer,
                                                       int dir, int view)
{

  ExtrudeParams *ep = new  ExtrudeParams;
  ep->mesh.BoundaryLayerIndex = dir;
  ep->mesh.ViewIndex = view; //view -5 for centerline based extrude
  ep->mesh.NbLayer = 1; //this may be more general for defining different layers
  ep->mesh.hLayer.clear();
  ep->mesh.hLayer.push_back(hLayer);
  ep->mesh.NbElmLayer.clear();
  ep->mesh.NbElmLayer.push_back(nbLayers);
  ep->mesh.ExtrudeMesh = true;
  if (CTX::instance()->mesh.recombineAll){
    ep->mesh.Recombine = true;
    //ep->mesh.QuadToTri = QUADTRI_DBL_1 ; //QUADTRI_SNGL_1_RECOMB; //QUADTRI_SNGL_1;
  }
  else ep->mesh.Recombine = false;
  ep->geo.Source = e->tag();

  int type  = BOUNDARY_LAYER;
  double T0=0., T1=0., T2=0.;
  double A0=0., A1=0., A2=0.;
  double X0=0., X1=0., X2=0.,alpha=0.;

  std::vector<GEntity*> extrudedEntities;

  //extrude shape dans geo.cpp
  Shape shape;
  if(e->dim() == 0){
    Vertex *v = FindPoint(e->tag());
    if(!v) printf("vertex %d not found \n", e->tag());
    shape.Num = v->Num;
    shape.Type = v->Typ;
  }
  else if (e->dim() == 1){
    ((GEdge*)e)->meshAttributes.extrude = ep;
    Curve *c = FindCurve(e->tag());
    if(!c) printf("curve %d not found \n", e->tag());
     shape.Num = c->Num;
     shape.Type = c->Typ;
  }
  else if (e->dim() == 2){
    ((GFace*)e)->meshAttributes.extrude = ep;
    Surface *s = FindSurface(e->tag());
    if(!s) {
      Msg::Error("Surface %d not found", e->tag());
      return extrudedEntities;
    }
    shape.Num = s->Num;
    shape.Type = s->Typ;
  }

  //extrude shape
  List_T *list_out= List_Create(2, 1, sizeof(Shape));
  List_T *tmp = List_Create(1, 1, sizeof(Shape));
  List_Add(tmp, &shape);
  ExtrudeShapes(type, tmp,
                T0, T1, T2,
                A0, A1, A2,
                X0, X1, X2, alpha,
                ep,
                list_out);

  //create GEntities
  gm->getGEOInternals()->synchronize(gm);

  //return the new created entity
  int nbout = List_Nbr(list_out);
  if(e->dim()==1){
    Shape e;
    Shape s;
    List_Read(list_out, 0, &e);
    List_Read(list_out, 1, &s);
    GEdge *ge = gm->getEdgeByTag(e.Num);
    GFace *gf = gm->getFaceByTag(s.Num);
    extrudedEntities.push_back((GEntity*)ge);
    extrudedEntities.push_back((GEntity*)gf);
    for (int j=2; j<nbout; j++){
      Shape el;
      List_Read(list_out, j, &el);
      GEdge *gel = gm->getEdgeByTag(el.Num);
      extrudedEntities.push_back((GEntity*)gel);
    }
  }
  else if(e->dim()==2){
    Shape s;
    Shape v;
    List_Read(list_out, 0, &s);
    List_Read(list_out, 1, &v);
    GFace *gf = gm->getFaceByTag(s.Num);
    GRegion *gr = gm->getRegionByTag(v.Num);
    extrudedEntities.push_back((GEntity*)gf);
    extrudedEntities.push_back((GEntity*)gr);
    for (int j=2; j<nbout; j++){
      Shape sl;
      List_Read(list_out, j, &sl);
      GFace *gfl = gm->getFaceByTag(sl.Num);
      extrudedEntities.push_back((GEntity*)gfl);
    }
  }

  List_Delete(list_out);
  List_Delete(tmp);

  return extrudedEntities;

  // //return the new created entity
  // GEntity *newEnt=0;
  // if (e->dim() == 1){
  //  for(GModel::eiter it = gm->firstEdge(); it != gm->lastEdge(); it++){
  //    GEdge *ge = *it;
  //   if(ge->getNativeType() == GEntity::GmshModel &&
  //      ge->geomType() == GEntity::BoundaryLayerCurve){
  //     ExtrudeParams *ep = ge->meshAttributes.extrude;
  //     if(ep && ep->mesh.ExtrudeMesh && ep->geo.Mode == COPIED_ENTITY &&
  //  std::abs(ep->geo.Source) ==e->tag() )
  //    newEnt = ge;
  //     }
  //   }
  // }
  // else if (e->dim() ==2){
  //   for(GModel::fiter it = gm->firstFace(); it != gm->lastFace(); it++){
  //     GFace *gf = *it;
  //     if(gf->getNativeType() == GEntity::GmshModel &&
  //   gf->geomType() == GEntity::BoundaryLayerSurface){
  //  ExtrudeParams *ep = gf->meshAttributes.extrude;
  //  if(ep && ep->mesh.ExtrudeMesh && ep->geo.Mode == COPIED_ENTITY
  //     && std::abs(ep->geo.Source) == e->tag())
  //    newEnt = gf;
  //     }
  //   }
  // }

  // return newEnt;

};

void GeoFactory::healGeometry(GModel *gm, double tolerance)
{
  GModel *current = GModel::current();
  GModel::setCurrent(gm);
  ReplaceAllDuplicatesNew(tolerance);
  gm->destroy();
  gm->getGEOInternals()->synchronize(gm);
  GModel::setCurrent(current);
}

GFace *GeoFactory::_addPlanarFace(GModel *gm, const std::vector<std::vector<GEdgeSigned> > &edges, bool orientEdges)
{

  //create line loops
  int nLoops = edges.size();
  std::vector<EdgeLoop *> vecLoops;
  for (int i=0; i< nLoops; i++){
    int ne=(int)edges[i].size();
    List_T *temp = List_Create(ne, ne, sizeof(int));
    for(int j = 0; j < ne; j++){
      const GEdgeSigned &signedEdge = edges[i][j];
      GEdge *ge = signedEdge.ge;
      int numEdge = ge->tag();
      //create curve if it does not exist
      Curve *c = FindCurve(numEdge);
      if(!c){
  GVertex *gvb = ge->getBeginVertex();
  GVertex *gve = ge->getEndVertex();
  Vertex *vertb = FindPoint(abs(gvb->tag()));
  Vertex *verte = FindPoint(abs(gve->tag()));
  if (!vertb){
    vertb = CreateVertex(gvb->tag(), gvb->x(), gvb->y(), gvb->z(),
        gvb->prescribedMeshSizeAtVertex(), 1.0);
    Tree_Add(gm->getGEOInternals()->Points, &vertb);
    vertb->Typ = MSH_POINT;
    vertb->Num = gvb->tag();
   }
  if (!verte){
    verte = CreateVertex(gve->tag(), gve->x(), gve->y(), gve->z(),
        gve->prescribedMeshSizeAtVertex(), 1.0);
    Tree_Add(gm->getGEOInternals()->Points, &verte);
    verte->Typ = MSH_POINT;
    verte->Num = gve->tag();
  }

  if (ge->geomType() == GEntity::Line){
    c = CreateCurve(numEdge, MSH_SEGM_LINE, 1, NULL, NULL, -1, -1, 0., 1.);
  }
  else if (ge->geomType() == GEntity::DiscreteCurve){
    c = CreateCurve(numEdge, MSH_SEGM_DISCRETE, 1, NULL, NULL, -1, -1, 0., 1.);
  }
  else if(ge->geomType() == GEntity::CompoundCurve){
    c = CreateCurve(numEdge, MSH_SEGM_COMPOUND, 1, NULL, NULL, -1, -1, 0., 1.);
    std::vector<GEdge*> gec = ((GEdgeCompound*)ge)->getCompounds();
    for(unsigned int i = 0; i < gec.size(); i++)
      c->compound.push_back(gec[i]->tag());
  }
  else{
    c = CreateCurve(numEdge, MSH_SEGM_DISCRETE, 1, NULL, NULL, -1, -1, 0., 1.);
  }

  c->Control_Points = List_Create(2, 1, sizeof(Vertex *));
  List_Add(c->Control_Points, &vertb);
  List_Add(c->Control_Points, &verte);
  c->beg = vertb;
  c->end = verte;
  EndCurve(c);

  Tree_Add(gm->getGEOInternals()->Curves, &c);
  CreateReversedCurve(c);
      }
      int signedNumEdge = numEdge*signedEdge.getSign();
      List_Add(temp, &signedNumEdge);
    }

    int numl = gm->getMaxElementaryNumber(1) + i;
    while (FindEdgeLoop(numl)){
      numl++;
      if (!FindEdgeLoop(numl)) break;
    }
    SortEdgesInLoop(numl, temp, orientEdges);
    EdgeLoop *l = CreateEdgeLoop(numl, temp);
    vecLoops.push_back(l);
    Tree_Add(gm->getGEOInternals()->EdgeLoops, &l);
    l->Num = numl;
    List_Delete(temp);
  }

  //create surface
  int numf  = gm->getMaxElementaryNumber(2)+1;
  Surface *s = CreateSurface(numf, MSH_SURF_PLAN);
  List_T *temp = List_Create(nLoops, nLoops, sizeof(int));
  for (int i = 0; i < nLoops; i++){
    int numl = vecLoops[i]->Num;
    List_Add(temp, &numl);
  }

  SetSurfaceGeneratrices(s, temp);
  List_Delete(temp);
  EndSurface(s);
  Tree_Add(gm->getGEOInternals()->Surfaces, &s);

  //gmsh surface
  GFace *gf = new gmshFace(gm,s);
  gm->add(gf);

  return gf;
}

#if defined(HAVE_OCC)
#include "GModelIO_OCC.h"
#include "OCCVertex.h"
#include "OCCEdge.h"
#include "OCCFace.h"
#include "OCCRegion.h"

#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepGProp.hxx>
#include <BRepLProp_SLProps.hxx>
#include <BRepLib.hxx>
#include <BRepMesh.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepOffsetAPI_MakeFilling.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepTools.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <ElCLib.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GProp_GProps.hxx>
#include <Geom2d_Curve.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESToBRep_Reader.hxx>
#include <Interface_Static.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangle.hxx>
#include <Poly_Triangulation.hxx>
#include <Precision.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <ShapeAnalysis_DataMapOfShapeListOfReal.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <ShapeAnalysis_ShapeTolerance.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <ShapeFix.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeUpgrade_ShellSewing.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_Version.hxx>
#include <StlAPI_Writer.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_DataMapOfIntegerShape.hxx>
#include <TopTools_DataMapOfShapeInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_MakePln.hxx>
#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_GTrsf.hxx>
#include <gp_Vec.hxx>

GVertex *OCCFactory::addVertex(GModel *gm, double x, double y, double z, double lc)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Pnt aPnt;
  aPnt = gp_Pnt(x, y, z);
  BRepBuilderAPI_MakeVertex mkVertex(aPnt);
  TopoDS_Vertex occv = mkVertex.Vertex();

  GVertex *vertex = gm->_occ_internals->addVertexToModel(gm, occv);

  lc *= CTX::instance()->geom.scalingFactor;
  if(lc == 0.) lc = MAX_LC; // no mesh size given at the point
  vertex->setPrescribedMeshSizeAtVertex(lc);

  return vertex;
}

GEdge *OCCFactory::addLine(GModel *gm, GVertex *start, GVertex *end)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  OCCVertex *occv1 = dynamic_cast<OCCVertex*>(start);
  OCCVertex *occv2 = dynamic_cast<OCCVertex*>(end);
  TopoDS_Edge occEdge;
  if (occv1 && occv2){
     occEdge = BRepBuilderAPI_MakeEdge(occv1->getShape(),
               occv2->getShape()).Edge();
  }
  else{
    gp_Pnt p1(start->x(),start->y(),start->z());
    gp_Pnt p2(end->x(),end->y(),end->z());
    occEdge = BRepBuilderAPI_MakeEdge(p1, p2).Edge();
  }
  return gm->_occ_internals->addEdgeToModel(gm,occEdge);
}

GEdge *OCCFactory::addCircleArc(GModel *gm, const arcCreationMethod &method,
                                GVertex *start, GVertex *end,
                                const SPoint3 &aPoint)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Pnt aP1(start->x(), start->y(), start->z());
  gp_Pnt aP2(aPoint.x(), aPoint.y(), aPoint.z());
  gp_Pnt aP3(end->x(), end->y(), end->z());
  TopoDS_Edge occEdge;

  OCCVertex *occv1 = dynamic_cast<OCCVertex*>(start);
  OCCVertex *occv2 = dynamic_cast<OCCVertex*>(end);

  if (method == GModelFactory::THREE_POINTS){
    GC_MakeArcOfCircle arc(aP1, aP2, aP3);
    if (occv1 && occv2)
      occEdge = BRepBuilderAPI_MakeEdge(arc.Value(),occv1->getShape(),
                                        occv2->getShape()).Edge();
    else
      occEdge = BRepBuilderAPI_MakeEdge(arc.Value()).Edge();
  }
  else if (method == GModelFactory::CENTER_START_END){
    Standard_Real Radius = aP1.Distance(aP2);
    gce_MakeCirc MC(aP2,gce_MakePln(aP1, aP2, aP3).Value(), Radius);
    const gp_Circ& Circ = MC.Value();
    Standard_Real Alpha1 = ElCLib::Parameter(Circ, aP1);
    Standard_Real Alpha2 = ElCLib::Parameter(Circ, aP3);
    Handle(Geom_Circle) C = new Geom_Circle(Circ);
    Handle(Geom_TrimmedCurve) arc = new Geom_TrimmedCurve(C, Alpha1, Alpha2, false);
    if (occv1 && occv2)
      occEdge = BRepBuilderAPI_MakeEdge(arc, occv1->getShape(),
                                        occv2->getShape()).Edge();
    else
      occEdge = BRepBuilderAPI_MakeEdge(arc).Edge();
  }
  return gm->_occ_internals->addEdgeToModel(gm,occEdge);
}

GEdge *OCCFactory::addSpline(GModel *gm, const splineType &type,
                             GVertex *start, GVertex *end,
           std::vector<std::vector<double> > points)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  TopoDS_Edge occEdge;

  OCCVertex *occv1 = dynamic_cast<OCCVertex*>(start);
  OCCVertex *occv2 = dynamic_cast<OCCVertex*>(end);

  int nbControlPoints = points.size();
  TColgp_Array1OfPnt ctrlPoints(1, nbControlPoints + 2);
  int index = 1;
  ctrlPoints.SetValue(index++, gp_Pnt(start->x(), start->y(), start->z()));

  //  printf("%d %d %d %d\n",points.size(),points[0].size(),points[1].size(),points[2].size());
  for (int i = 0; i < nbControlPoints; i++) {
    //    printf("%g %g %g\n",points[i][0],points[i][1],points[i][2]);
    gp_Pnt aP(points[i][0],points[i][1],points[i][2]);
    ctrlPoints.SetValue(index++, aP);
  }
  ctrlPoints.SetValue(index++, gp_Pnt(end->x(), end->y(), end->z()));
  if (type == BEZIER) {
    if (nbControlPoints >= 20)Msg::Fatal("OCC Bezier Curves have a maximum degree of 20");
    Handle(Geom_BezierCurve) Bez = new Geom_BezierCurve(ctrlPoints);
    if (occv1 && occv2)
      occEdge = BRepBuilderAPI_MakeEdge(Bez,occv1->getShape(),occv2->getShape()).Edge();
    else
      occEdge = BRepBuilderAPI_MakeEdge(Bez).Edge();
  }
  else if (type == BSPLINE) {

    Handle(Geom_BSplineCurve) Bez = GeomAPI_PointsToBSpline(ctrlPoints).Curve();

    if (occv1 && occv2)
      occEdge = BRepBuilderAPI_MakeEdge(Bez,occv1->getShape(),occv2->getShape()).Edge();
    else
      occEdge = BRepBuilderAPI_MakeEdge(Bez).Edge();
  }


  return gm->_occ_internals->addEdgeToModel(gm, occEdge);
}


GEdge *OCCFactory::addNURBS(GModel *gm, GVertex *start, GVertex *end,
          std::vector<std::vector<double> > points,
          std::vector<double> knots,
          std::vector<double> weights,
          std::vector<int> mult)
{
  try{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  OCCVertex *occv1 = dynamic_cast<OCCVertex*>(start);
  OCCVertex *occv2 = dynamic_cast<OCCVertex*>(end);

  int nbControlPoints = points.size() + 2;
  TColgp_Array1OfPnt  ctrlPoints(1, nbControlPoints);

  TColStd_Array1OfReal _knots(1, knots.size());
  TColStd_Array1OfReal _weights(1, weights.size());
  TColStd_Array1OfInteger  _mult(1, mult.size());

  for (unsigned i = 0; i < knots.size(); i++) {
    _knots.SetValue(i+1, knots[i]);
  }
  for (unsigned i = 0; i < weights.size(); i++) {
    _weights.SetValue(i+1, weights[i]);
  }
  int totKnots = 0;
  for (unsigned i = 0; i < mult.size(); i++) {
    _mult.SetValue(i+1, mult[i]);
    totKnots += mult[i];
  }

  const int degree = totKnots - nbControlPoints - 1;
  Msg::Debug("creation of a nurbs of degree %d with %d control points",
       degree,nbControlPoints);

  int index = 1;
  ctrlPoints.SetValue(index++, gp_Pnt(start->x(), start->y(), start->z()));
  for (unsigned i = 0; i < points.size(); i++) {
    gp_Pnt aP(points[i][0],points[i][1],points[i][2]);
    ctrlPoints.SetValue(index++, aP);
  }
  ctrlPoints.SetValue(index++, gp_Pnt(end->x(), end->y(), end->z()));
  Handle(Geom_BSplineCurve) NURBS = new Geom_BSplineCurve
    (ctrlPoints, _weights, _knots, _mult, degree, false);
  TopoDS_Edge occEdge;
  if (occv1 && occv2)
    occEdge = BRepBuilderAPI_MakeEdge(NURBS, occv1->getShape(),
                                      occv2->getShape()).Edge();
  else
    occEdge = BRepBuilderAPI_MakeEdge(NURBS).Edge();
  return gm->_occ_internals->addEdgeToModel(gm, occEdge);
  }
  catch(Standard_Failure &err){
    Msg::Error("%s", err.GetMessageString());
  }
  return 0;
}

/* add2Drect: rectangluar face, given lower left point and width/height (in X-Y plane)*/
GFace *OCCFactory::add2Drect(GModel *gm,double x0, double y0, double dx, double dy)
{
  Msg::Info("Default working plane is XY in add2D* functions...");

  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  TopoDS_Vertex occv1 = BRepBuilderAPI_MakeVertex( gp_Pnt(x0   , y0   , 0.) );
  TopoDS_Vertex occv2 = BRepBuilderAPI_MakeVertex( gp_Pnt(x0+dx, y0   , 0.) );
  TopoDS_Vertex occv3 = BRepBuilderAPI_MakeVertex( gp_Pnt(x0+dx, y0+dy, 0.) );
  TopoDS_Vertex occv4 = BRepBuilderAPI_MakeVertex( gp_Pnt(x0   , y0+dy, 0.) );

  TopoDS_Edge occEdge1 = BRepBuilderAPI_MakeEdge(occv1,occv2);
  TopoDS_Edge occEdge2 = BRepBuilderAPI_MakeEdge(occv2,occv3);
  TopoDS_Edge occEdge3 = BRepBuilderAPI_MakeEdge(occv3,occv4);
  TopoDS_Edge occEdge4 = BRepBuilderAPI_MakeEdge(occv4,occv1);

  TopoDS_Wire rectWire = BRepBuilderAPI_MakeWire(occEdge1 , occEdge2 , occEdge3, occEdge4);
  TopoDS_Face rectFace = BRepBuilderAPI_MakeFace(rectWire);

  return gm->_occ_internals->addFaceToModel(gm, TopoDS::Face(rectFace));
}

/* add2Dellips: ellips face, given lower left point and width/height (in X-Y plane)*/
GFace *OCCFactory::add2Dellips(GModel *gm, double xc, double yc, double rx, double ry)
{
  Msg::Info("Default working plane is XY in add2D* functions...");

  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Dir N_dir(0., 0., 1.);
  gp_Dir x_dir(1., 0., 0.);
  gp_Pnt center(xc, yc, 0.);
  gp_Ax2 axis(center, N_dir, x_dir);
  gp_Elips ellipse = gp_Elips(axis, rx, ry);
  // gp_Ax2(gp_Pnt(xc,yc;0),gp_Dir(0.,0.,1.)),rx,ry
  TopoDS_Edge ellipsEdge = BRepBuilderAPI_MakeEdge(ellipse);
  TopoDS_Wire ellipsWire = BRepBuilderAPI_MakeWire(ellipsEdge);
  TopoDS_Face ellipsFace = BRepBuilderAPI_MakeFace(ellipsWire);
  // TopoDS_Edge ellipsEdge = BRepBuilderAPI_MakeEdge( gp_Elips2d(gp_Ax22d(gp_Pnt2d(xc,yc),
  //                                                   gp_Dir2d(1.,0.), gp_Dir2d(0.,1.)),rx,ry) );

  return gm->_occ_internals->addFaceToModel(gm, TopoDS::Face(ellipsFace));
}

GEntity *OCCFactory::revolve(GModel *gm, GEntity* base,
                             std::vector<double> p1,
                             std::vector<double> p2, double angle)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];

  gp_Dir direction(x2 - x1, y2 - y1, z2 - z1);
  gp_Ax1 axisOfRevolution(gp_Pnt(x1, y1, z1), direction);
  BRepPrimAPI_MakeRevol MR(*(TopoDS_Shape*)base->getNativePtr(),
                           axisOfRevolution, angle, Standard_False);
  GEntity *ret = 0;
  if (base->cast2Vertex()){
    TopoDS_Edge result = TopoDS::Edge(MR.Shape());
    ret = gm->_occ_internals->addEdgeToModel(gm, result);
  }
  if (base->cast2Edge()){
    TopoDS_Face result = TopoDS::Face(MR.Shape());
    ret = gm->_occ_internals->addFaceToModel(gm, result);
  }
  if (base->cast2Face()){
    TopoDS_Solid result = TopoDS::Solid(MR.Shape());
    ret = gm->_occ_internals->addRegionToModel(gm, result);
  }
  return ret;
}

GEntity *OCCFactory::extrude(GModel *gm, GEntity* base,
                             std::vector<double> p1, std::vector<double> p2)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];

  gp_Vec direction(gp_Pnt(x1, y1, z1), gp_Pnt(x2, y2, z2));
  BRepPrimAPI_MakePrism MP(*(TopoDS_Shape*)base->getNativePtr(), direction,
                           Standard_False);
  GEntity *ret = 0;
  if (base->cast2Vertex()){
    TopoDS_Edge result = TopoDS::Edge(MP.Shape());
    ret = gm->_occ_internals->addEdgeToModel(gm, result);
  }
  if (base->cast2Edge()){
    TopoDS_Face result = TopoDS::Face(MP.Shape());
    ret = gm->_occ_internals->addFaceToModel(gm, result);
  }
  if (base->cast2Face()){
    TopoDS_Solid result = TopoDS::Solid(MP.Shape());
    ret = gm->_occ_internals->addRegionToModel(gm, result);
  }
  return ret;
}

GEntity *OCCFactory::addSphere(GModel *gm, double xc, double yc, double zc, double radius)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Pnt aP(xc, yc, zc);
  TopoDS_Shape shape = BRepPrimAPI_MakeSphere(aP, radius).Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(shape));
}






GEntity* OCCFactory::addShape(GModel *gm,TopoDS_Shape S)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gm->_occ_internals->buildShapeFromLists(S);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(S));
}







GRegion* OCCFactory::addVolume (GModel *gm, std::vector<std::vector<GFace *> > faces)
{
  Msg::Error("add Volume not implemented yet for OCCFactory");
  return 0;
}

GEntity *OCCFactory::addCylinder(GModel *gm, std::vector<double> p1,
                                 std::vector<double> p2, double radius)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];

  gp_Pnt aP(x1, y1, z1);
  const double H = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) +
                        (z2 - z1) * (z2 - z1));
  gp_Vec aV((x2 - x1) / H, (y2 - y1) / H, (z2 - z1) / H);
  gp_Ax2 anAxes(aP, aV);
  BRepPrimAPI_MakeCylinder MC(anAxes, radius, H);
  MC.Build();
  if (!MC.IsDone()) {
    Msg::Error("Cylinder can't be computed from the given parameters");
    return 0;
  }
  TopoDS_Shape shape = MC.Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(shape));
}

GEntity *OCCFactory::addTorus(GModel *gm, std::vector<double> p1,
                              std::vector<double> p2, double radius1,
                              double radius2)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];
  gp_Pnt aP(x1, y1, z1);
  const double H = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) +
                        (z2 - z1) * (z2 - z1));
  gp_Vec aV((x2 - x1) / H, (y2 - y1) / H, (z2 - z1) / H);
  gp_Ax2 anAxes(aP, aV);
  BRepPrimAPI_MakeTorus MC(anAxes, radius1, radius2);
  MC.Build();
  if (!MC.IsDone()) {
    Msg::Error("Cylinder can't be computed from the given parameters");
    return 0;
  }
  TopoDS_Shape shape = MC.Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(shape));
}

GEntity *OCCFactory::addCone(GModel *gm,  std::vector<double> p1,
                             std::vector<double> p2, double radius1,
                             double radius2)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];

  gp_Pnt aP(x1, y1, z1);
  const double H = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) +
                        (z2 - z1) * (z2 - z1));
  gp_Vec aV((x2 - x1) / H, (y2 - y1) / H, (z2 - z1) / H);
  gp_Ax2 anAxes(aP, aV);
  BRepPrimAPI_MakeCone MC(anAxes, radius1, radius2, H);
  MC.Build();
  if (!MC.IsDone()) {
    Msg::Error("Cylinder can't be computed from the given parameters");
    return 0;
  }
  TopoDS_Shape shape = MC.Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm,TopoDS::Solid(shape));
}

GEntity *OCCFactory::addBlock(GModel *gm, std::vector<double> p1,
                              std::vector<double> p2)
{  
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Pnt P1(p1[0], p1[1], p1[2]);
  gp_Pnt P2(p2[0], p2[1], p2[2]);
  BRepPrimAPI_MakeBox MB(P1, P2);
  MB.Build();
  if (!MB.IsDone()) {
    Msg::Error("Box can not be computed from the given point");
    return 0;
  }
  TopoDS_Shape shape = MB.Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(shape));
}

GEntity *OCCFactory::add3DBlock(GModel *gm,std::vector<double> p1,
                                double dx, double dy, double dz)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Pnt P1(p1[0], p1[1], p1[2]);
  BRepPrimAPI_MakeBox MB(P1, dx, dy, dz);
  MB.Build();
  if (!MB.IsDone()) {
    Msg::Error("Box can not be computed from the given point");
    return 0;
  }
  TopoDS_Shape shape = MB.Shape();
  gm->_occ_internals->buildShapeFromLists(shape);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
  return gm->_occ_internals->getRegionForOCCShape(gm, TopoDS::Solid(shape));
}

GModel *OCCFactory::computeBooleanUnion(GModel* obj, GModel* tool,
                                        int createNewModel)
{
  try{
    OCC_Internals *occ_obj = obj->getOCCInternals();
    OCC_Internals *occ_tool = tool->getOCCInternals();

    if (!occ_obj || !occ_tool) return NULL;

    if (createNewModel){
      GModel *temp = new GModel;
      temp->_occ_internals = new OCC_Internals;
      temp->_occ_internals->_addShapeToLists(occ_obj->getShape());
      obj = temp;
    }
    obj->_occ_internals->applyBooleanOperator(occ_tool->getShape(),
                                              OCC_Internals::Union);
    obj->destroy();
    obj->_occ_internals->buildLists();
    obj->_occ_internals->buildGModel(obj);
  }
  catch(Standard_Failure &err){
    Msg::Error("%s", err.GetMessageString());
  }

  return obj;
}

GModel *OCCFactory::computeBooleanDifference(GModel* obj, GModel* tool,
                                             int createNewModel)
{
  try{
    OCC_Internals *occ_obj = obj->getOCCInternals();
    OCC_Internals *occ_tool = tool->getOCCInternals();

    if (!occ_obj || !occ_tool) return NULL;

    if (createNewModel){
      GModel *temp = new GModel;
      temp->_occ_internals = new OCC_Internals;
      temp->_occ_internals->_addShapeToLists(occ_obj->getShape());
      obj = temp;
    }
    obj->getOCCInternals()->applyBooleanOperator(occ_tool->getShape(),
                                                 OCC_Internals::Difference);
    obj->destroy();
    obj->_occ_internals->buildLists();
    obj->_occ_internals->buildGModel(obj);
  }
  catch(Standard_Failure &err){
    Msg::Error("%s", err.GetMessageString());
  }
  return obj;
}

GModel *OCCFactory::computeBooleanIntersection(GModel* obj, GModel* tool,
                                               int createNewModel)
{
  try{
    OCC_Internals *occ_obj = obj->getOCCInternals();
    OCC_Internals *occ_tool = tool->getOCCInternals();

    if (!occ_obj || !occ_tool) return NULL;

    if (createNewModel){
      GModel *temp = new GModel;
      temp->_occ_internals = new OCC_Internals;
      temp->_occ_internals->_addShapeToLists(occ_obj->getShape());
      obj = temp;
    }
    obj->getOCCInternals()->applyBooleanOperator(occ_tool->getShape(),
                                                 OCC_Internals::Intersection);
    obj->destroy();
    obj->_occ_internals->buildLists();
    obj->_occ_internals->buildGModel(obj);
  }
  catch(Standard_Failure &err){
    Msg::Error("%s", err.GetMessageString());
  }
  return obj;
}

/* IsEqualG : a tolerance function for setPeriodicAllFaces */
bool IsEqualG(double x, double y)
{
  const double epsilon = 0.00000000001;
  return fabs(x - y) <= epsilon * (std::max(fabs(x),fabs(y)));
}

/* setPeriodicAllFaces: detects T-periodic pair of faces and uses
   setPeriodicPairOfFaces to make them all periodic */
void OCCFactory::setPeriodicAllFaces(GModel *gm, std::vector<double> FaceTranslationVector)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  Msg::Info("Experimental: search for 'translated' faces and calls setPeriodicPairOfFaces automatically");
  TopoDS_Shape shape = gm->_occ_internals->getShape();
  gp_Trsf theTransformation;
  gp_Vec theVectorOfTranslation(FaceTranslationVector[0],
                                FaceTranslationVector[1],
                                FaceTranslationVector[2]);
  int               numFaceMaster   , numFaceSlave;
  std::list<GEdge*> ListOfEdgeMaster, ListOfEdgeSlave;
  for(TopExp_Explorer aFaceExplorer(shape, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()){
    TopoDS_Face aFace1 = TopoDS::Face(aFaceExplorer.Current());
    for(TopExp_Explorer aFaceExplorer2(shape, TopAbs_FACE); aFaceExplorer2.More(); aFaceExplorer2.Next()){
      TopoDS_Face aFace2 = TopoDS::Face(aFaceExplorer2.Current());
      // Get number of Edges
      int NumberOfEdgesInFace1=0;
      int NumberOfEdgesInFace2=0;
      for (TopExp_Explorer aEdgeExplorer1(aFace1,TopAbs_EDGE); aEdgeExplorer1.More();
           aEdgeExplorer1.Next()) {NumberOfEdgesInFace1++;}
      for (TopExp_Explorer aEdgeExplorer2(aFace2,TopAbs_EDGE); aEdgeExplorer2.More();
           aEdgeExplorer2.Next()) {NumberOfEdgesInFace2++;}
      // Get Surfaces of Faces (occtopo=>occgeom)
      Handle(Geom_Surface) Surf1=BRep_Tool::Surface(aFace1);
      Handle(Geom_Surface) Surf2=BRep_Tool::Surface(aFace2);
      // Get barycenter and area
      GProp_GProps FaceProps1;
      GProp_GProps FaceProps2;
      BRepGProp::SurfaceProperties(aFace1,FaceProps1);
      BRepGProp::SurfaceProperties(aFace2,FaceProps2);
      gp_Pnt BarycenterFace1 = FaceProps1.CentreOfMass ();
      gp_Pnt BarycenterFace2 = FaceProps2.CentreOfMass ();
      Standard_Real Area1 = FaceProps1.Mass();
      Standard_Real Area2 = FaceProps2.Mass();
      // Get surface normal
      //Standard_Real umin, vmin;//umax,vmax
      //GeomLProp_SLProps props1(Surf1, umin, vmin, 1, 0.01);
      //GeomLProp_SLProps props2(Surf2, umin, vmin, 1, 0.01);
      //gp_Dir norm1=props1.Normal();
      //gp_Dir norm2=props2.Normal();

      if(NumberOfEdgesInFace1==NumberOfEdgesInFace2
         && IsEqualG(Area1,Area2)
         && IsEqualG(BarycenterFace1.X()+FaceTranslationVector[0],BarycenterFace2.X())
         && IsEqualG(BarycenterFace1.Y()+FaceTranslationVector[1],BarycenterFace2.Y())
         && IsEqualG(BarycenterFace1.Z()+FaceTranslationVector[2],BarycenterFace2.Z())){
        numFaceMaster = gm->getOCCInternals()->getFaceForOCCShape(gm,aFace1)->tag();
        numFaceSlave  = gm->getOCCInternals()->getFaceForOCCShape(gm,aFace2)->tag();
        //Msg::Info("Face %d (slave) is most likely Face %d (master) translated by (%.2e,%.2e,%.2e)!",
        //           numFaceSlave,numFaceMaster,FaceTranslationVector[0],FaceTranslationVector[1],
        //           FaceTranslationVector[2]);

        std::vector<int>  EdgeListMaster(NumberOfEdgesInFace1);
        std::vector<int>  EdgeListSlave(NumberOfEdgesInFace2);
        int i1=0;//,i2=0;

        // ici il faut imbriquer la seconde boucle pour fournir des edges qui match slave/master
        for (TopExp_Explorer aEdgeExplorer1(aFace1,TopAbs_EDGE); aEdgeExplorer1.More();
             aEdgeExplorer1.Next()) {
          TopoDS_Edge aEdge1 = TopoDS::Edge(aEdgeExplorer1.Current());
          int numEdgeMaster  = gm->getOCCInternals()->getEdgeForOCCShape(gm,aEdge1)->tag();
          EdgeListMaster[i1] = numEdgeMaster;
          //i2=0;
          for (TopExp_Explorer aEdgeExplorer2(aFace2,TopAbs_EDGE); aEdgeExplorer2.More();
               aEdgeExplorer2.Next()) {
            TopoDS_Edge aEdge2 = TopoDS::Edge(aEdgeExplorer2.Current());
            GProp_GProps EdgeProps1;
            GProp_GProps EdgeProps2;
            BRepGProp::LinearProperties(aEdge1, EdgeProps1);
            BRepGProp::LinearProperties(aEdge2, EdgeProps2);
            gp_Pnt BarycenterEdge1 = EdgeProps1.CentreOfMass ();
            gp_Pnt BarycenterEdge2 = EdgeProps2.CentreOfMass ();
            Standard_Real Length1 = EdgeProps1.Mass();
            Standard_Real Length2 = EdgeProps2.Mass();
            if(IsEqualG(Length1,Length2)
               && IsEqualG(BarycenterEdge1.X()+FaceTranslationVector[0],BarycenterEdge2.X())
               && IsEqualG(BarycenterEdge1.Y()+FaceTranslationVector[1],BarycenterEdge2.Y())
               && IsEqualG(BarycenterEdge1.Z()+FaceTranslationVector[2],BarycenterEdge2.Z())){
              int numEdgeSlave   = gm->getOCCInternals()->getEdgeForOCCShape(gm,aEdge2)->tag();
              EdgeListSlave[i1]  = numEdgeSlave;
            }
          }
          i1++;
        }
        if(NumberOfEdgesInFace1==4)
          Msg::Info("Setting Periodic : Face %d  {%d,%d,%d,%d} (slave) and Face %d "
                    "{%d,%d,%d,%d} (master)",
                    numFaceSlave ,EdgeListSlave[0] ,EdgeListSlave[1] ,EdgeListSlave[2],
                    EdgeListSlave[3], numFaceMaster,EdgeListMaster[0],EdgeListMaster[1],
                    EdgeListMaster[2],EdgeListMaster[3]);
        setPeriodicPairOfFaces(gm, numFaceMaster, EdgeListMaster, numFaceSlave, EdgeListSlave);
      }
      else ;
    }
  }
}

/* setPeriodicPairOfFaces: set periodic given a Slave/Master pair of
   numFace-Edgelist */
void OCCFactory::setPeriodicPairOfFaces(GModel *gm, int numFaceMaster,
                                        std::vector<int> EdgeListMaster,
                                        int numFaceSlave,
                                        std::vector<int> EdgeListSlave)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  if (EdgeListMaster.size() != EdgeListSlave.size()){
    Msg::Error("Slave/Master faces don't have the same number of edges!");
  }
  else {

    std::map<int,int> edgeCounterparts;
    std::vector<int>::iterator siter = EdgeListSlave.begin();
    std::vector<int>::iterator miter = EdgeListMaster.begin();

    for (;siter!=EdgeListSlave.end();++siter,++miter) {
      edgeCounterparts[*siter] = *miter;
    }

    GFace *gf = GModel::current()->getFaceByTag(abs(numFaceSlave));
    if (gf) {
      GFace *master = GModel::current()->getFaceByTag(abs(numFaceMaster));
      gf->setMeshMaster(master,edgeCounterparts);
    }
    else Msg::Error("Slave surface %d not found", numFaceSlave);
  }
}

void OCCFactory::fillet(GModel *gm, std::vector<int> edges, double radius)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  try{
    std::vector<TopoDS_Edge> edgesToFillet;
    for (unsigned i = 0; i < edges.size(); i++){
      GEdge *ed = gm->getEdgeByTag(edges[i]);
      if (ed){
  OCCEdge *occed = dynamic_cast<OCCEdge*>(ed);
  if (occed)edgesToFillet.push_back(occed->getTopoDS_Edge());
      }
    }
    gm->_occ_internals->fillet(edgesToFillet, radius);
    gm->destroy();
    gm->_occ_internals->buildLists();
    gm->_occ_internals->buildGModel(gm);
  }
  catch(Standard_Failure &err){
    Msg::Error("%s", err.GetMessageString());
  }
}

void OCCFactory::translate(GModel *gm, std::vector<double> dx, int addToTheModel)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  gp_Trsf transformation;
  transformation.SetTranslation(gp_Pnt (0,0,0),gp_Pnt (dx[0],dx[1],dx[2]));
  BRepBuilderAPI_Transform aTransformation(gm->_occ_internals->getShape(),
                                           transformation, Standard_False);
  TopoDS_Shape temp = aTransformation.Shape();
  if (!addToTheModel) gm->_occ_internals->loadShape(& temp);
  else gm->_occ_internals->buildShapeFromLists(temp);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
}

void OCCFactory::rotate(GModel *gm, std::vector<double> p1, std::vector<double> p2,
                        double angle, int addToTheModel)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x1 = p1[0];
  const double y1 = p1[1];
  const double z1 = p1[2];
  const double x2 = p2[0];
  const double y2 = p2[1];
  const double z2 = p2[2];

  gp_Trsf transformation;

  gp_Vec direction(gp_Pnt(x1, y1, z1), gp_Pnt(x2, y2, z2));
  gp_Ax1 axisOfRevolution(gp_Pnt(x1, y1, z1), direction);
  transformation.SetRotation(axisOfRevolution, angle);
  BRepBuilderAPI_Transform aTransformation(gm->_occ_internals->getShape(),
                                           transformation, Standard_False);
  TopoDS_Shape temp = aTransformation.Shape();
  if (!addToTheModel) gm->_occ_internals->loadShape(&temp);
  else gm->_occ_internals->buildShapeFromLists(temp);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
}

void OCCFactory::dilate(GModel *gm, std::vector<double> p, std::vector<double> s,
                        int addToTheModel)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  const double x = p[0];
  const double y = p[1];
  const double z = p[2];

  const double a = s[0];
  const double b = s[1];
  const double c = s[2];

  gp_GTrsf transformation;
  transformation.SetVectorialPart(gp_Mat(a, 0, 0, 0, b, 0, 0, 0, c));
  transformation.SetTranslationPart(gp_XYZ(x,y,z));
  BRepBuilderAPI_GTransform aTransformation(gm->_occ_internals->getShape(),
                                           transformation, Standard_False);
  TopoDS_Shape temp = aTransformation.Shape();
  if (!addToTheModel) gm->_occ_internals->loadShape(&temp);
  else gm->_occ_internals->buildShapeFromLists(temp);
  gm->destroy();
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
}

std::vector<GFace *> OCCFactory::addRuledFaces(GModel *gm,
                                               std::vector< std::vector<GEdge *> > wires)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  std::vector<GFace*> faces;
  Standard_Boolean anIsSolid = Standard_False;
  Standard_Boolean anIsRuled = Standard_True;
  BRepOffsetAPI_ThruSections aGenerator (anIsSolid,anIsRuled);

  for (unsigned i=0;i<wires.size();i++) {
    BRepBuilderAPI_MakeWire wire_maker;
    for (unsigned j=0;j<wires[i].size();j++) {
      GEdge *ge = wires[i][j];
      OCCEdge *occe = dynamic_cast<OCCEdge*>(ge);
      if (occe){
  wire_maker.Add(occe->getTopoDS_Edge());
      }
    }
    aGenerator.AddWire (wire_maker.Wire());
  }

  aGenerator.CheckCompatibility (Standard_False);

  aGenerator.Build();

  TopoDS_Shape aResult = aGenerator.Shape();

  TopExp_Explorer exp2;
  for(exp2.Init(TopoDS::Shell(aResult), TopAbs_FACE); exp2.More(); exp2.Next()){
    TopoDS_Face face = TopoDS::Face(exp2.Current());
    GFace *ret = gm->_occ_internals->addFaceToModel(gm, face);
    faces.push_back(ret);
  }
  return faces;
}

GFace *OCCFactory::addFace(GModel *gm, std::vector<GEdge *> edges,
                           std::vector< std::vector<double > > points)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  BRepOffsetAPI_MakeFilling aGenerator;

  for (unsigned i = 0; i < edges.size(); i++) {
    GEdge *ge = edges[i];
    OCCEdge *occe = dynamic_cast<OCCEdge*>(ge);
    if (occe){
      aGenerator.Add(occe->getTopoDS_Edge(), GeomAbs_C0);
    }
  }
  for (unsigned i = 0; i < points.size(); i++) {
    gp_Pnt aPnt(points[i][0], points[i][1], points[i][2]);
    aGenerator.Add(aPnt);
  }

  aGenerator.Build();

  TopoDS_Shape aResult = aGenerator.Shape();
  return gm->_occ_internals->addFaceToModel(gm, TopoDS::Face(aResult));
}

GFace *OCCFactory::addPlanarFace(GModel *gm, std::vector< std::vector<GEdge *> > wires)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  std::set<GVertex*> verts;
  for (unsigned i = 0; i < wires.size(); i++) {
    for (unsigned j = 0; j < wires[i].size(); j++) {
      GEdge *ge = wires[i][j];
      verts.insert(ge->getBeginVertex());
      verts.insert(ge->getEndVertex());
    }
  }
  std::vector<SPoint3> points;
  std::set<GVertex*>::iterator it = verts.begin();
  for ( ; it != verts.end(); ++it){
    points.push_back(SPoint3((*it)->x(), (*it)->y(), (*it)->z()));
  }

  mean_plane meanPlane;
  computeMeanPlaneSimple(points, meanPlane);

  gp_Pln aPlane (meanPlane.a, meanPlane.b, meanPlane.c, -meanPlane.d);
  BRepBuilderAPI_MakeFace aGenerator (aPlane);

  for (unsigned i = 0; i < wires.size() ;i++) {
    BRepBuilderAPI_MakeWire wire_maker;
    for (unsigned j = 0; j < wires[i].size(); j++) {
      GEdge *ge = wires[i][j];
      OCCEdge *occe = dynamic_cast<OCCEdge*>(ge);
      if (occe){
  wire_maker.Add(occe->getTopoDS_Edge());
      }
    }
    TopoDS_Wire myWire = wire_maker.Wire();
    //if (i)myWire.Reverse();
    aGenerator.Add (myWire);
  }

  aGenerator.Build();
  TopoDS_Shape aResult = aGenerator.Shape();

  return gm->_occ_internals->addFaceToModel(gm, TopoDS::Face(aResult));
}

GFace *OCCFactory::addPlanarFace(GModel *gm, const std::vector<std::vector<GEdgeSigned> > &edges)
{
  Msg::Error("addPlanarFace with oriented edges not implemented yet for OCCFactory");
  return 0;
}

GEntity *OCCFactory::addPipe(GModel *gm, GEntity *base, std::vector<GEdge *> wire)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  BRepBuilderAPI_MakeWire wire_maker;
  for (unsigned j = 0; j < wire.size(); j++) {
    GEdge *ge = wire[j];
    OCCEdge *occe = dynamic_cast<OCCEdge*>(ge);
    if (occe){
      wire_maker.Add(occe->getTopoDS_Edge());
    }
  }
  TopoDS_Wire myWire = wire_maker.Wire();

  GEntity *ret = 0;
  if (base->cast2Vertex()){
    OCCVertex *occv = dynamic_cast<OCCVertex*>(base);
    BRepOffsetAPI_MakePipe myNiceLittlePipe (myWire, occv->getShape());
    TopoDS_Edge result = TopoDS::Edge(myNiceLittlePipe.Shape());
    ret = gm->_occ_internals->addEdgeToModel(gm, result);
  }
  if (base->cast2Edge()){
    OCCEdge *occe = dynamic_cast<OCCEdge*>(base);
    BRepOffsetAPI_MakePipe myNiceLittlePipe (myWire, occe->getTopoDS_Edge());
    TopoDS_Face result = TopoDS::Face(myNiceLittlePipe.Shape());
    ret = gm->_occ_internals->addFaceToModel(gm, result);
  }
  if (base->cast2Face()){
    OCCFace *occf = dynamic_cast<OCCFace*>(base);
    BRepOffsetAPI_MakePipe myNiceLittlePipe (myWire, occf->getTopoDS_Face());
    TopoDS_Solid result = TopoDS::Solid(myNiceLittlePipe.Shape());
    ret = gm->_occ_internals->addRegionToModel(gm, result);
  }
  return ret;
}

GEntity *OCCFactory::addThruSections(GModel *gm, std::vector<std::vector<GEdge *> > wire)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  BRepOffsetAPI_ThruSections aGenerator(Standard_True); // create solid
  for (unsigned i = 0; i < wire.size(); i++) {
    BRepBuilderAPI_MakeWire wire_maker;
    for (unsigned j = 0; j < wire[i].size(); j++) {
      GEdge *ge = wire[i][j];
      OCCEdge *occe = dynamic_cast<OCCEdge*>(ge);
      if (occe){
        wire_maker.Add(occe->getTopoDS_Edge());
      }
    }
    aGenerator.AddWire(wire_maker.Wire());
  }
  aGenerator.CheckCompatibility(Standard_False);
  aGenerator.Build();
  TopoDS_Solid result = TopoDS::Solid(aGenerator.Shape());
  GEntity *ret = gm->_occ_internals->addRegionToModel(gm, result);
  return ret;
}

void OCCFactory::healGeometry(GModel *gm, double tolerance)
{
  if (!gm->_occ_internals)
    gm->_occ_internals = new OCC_Internals;

  if (tolerance < 0.)
    tolerance = CTX::instance()->geom.tolerance;
  if (!gm || !gm->_occ_internals)
    return;
  gm->_occ_internals->_healGeometry(tolerance, true, true, true, true, true, true);
  gm->_occ_internals->buildLists();
  gm->_occ_internals->buildGModel(gm);
}

#endif
