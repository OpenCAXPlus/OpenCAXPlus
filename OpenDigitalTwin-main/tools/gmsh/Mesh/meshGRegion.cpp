// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <stdlib.h>
#include <vector>
#include "GmshConfig.h"
#include "GmshMessage.h"
#include "meshGRegion.h"
#include "meshGFace.h"
#include "meshGFaceOptimize.h"
#include "boundaryLayersData.h"
#include "meshGRegionBoundaryRecovery.h"
#include "meshGRegionDelaunayInsertion.h"
#include "meshGRegionRelocateVertex.h"
#include "GModel.h"
#include "GRegion.h"
#include "GFace.h"
#include "GEdge.h"
#include "gmshRegion.h"
#include "MLine.h"
#include "MPoint.h"
#include "MTriangle.h"
#include "MQuadrangle.h"
#include "MTetrahedron.h"
#include "MPyramid.h"
#include "MPrism.h"
#include "MHexahedron.h"
#include "BDS.h"
#include "OS.h"
#include "Context.h"
#include "GFaceCompound.h"
#include "meshGRegionMMG3D.h"
#include "simple3D.h"
#include "directions3D.h"
#include "pointInsertion.h"
#include "Levy3D.h"
#include "discreteFace.h"
#include "filterElements.h"
#include "ExtrudeParams.h"

#if defined(HAVE_ANN)
#include "ANN/ANN.h"
#endif

// hybrid mesh recovery structure
class splitQuadRecovery {
  std::multimap<GEntity*, std::pair<MVertex*,MFace> >_data;
  bool _empty;
 public :
  std::map<MFace, MVertex*, Less_Face>_invmap;
  std::set<MFace, Less_Face>_toDelete;
  splitQuadRecovery() : _empty(true) {}
  bool empty(){ return _empty; }
  void setEmpty(bool val){ _empty = val; }
  void add (const MFace &f, MVertex *v, GEntity*ge)
  {
    _data.insert(std::make_pair(ge, std::make_pair(v,f)));
  }
  void relocateVertices(GRegion *region, int niter) {
    if(empty()) return ;
    v2t_cont adj;
    buildVertexToElement(region->tetrahedra, adj);
    buildVertexToElement(region->pyramids, adj);
    buildVertexToElement(region->prisms, adj);
    buildVertexToElement(region->hexahedra, adj);

    double minQual     = 1;
    double minQualOpti = 1;

    std::list<GFace*> faces = region->faces();

    for (int iter=0; iter < niter+2;iter++){
      for (std::list<GFace*>::iterator it = faces.begin(); it != faces.end(); ++it){
	for (std::multimap<GEntity*, std::pair<MVertex*,MFace> >::iterator it2 =
	       _data.lower_bound(*it); it2 != _data.upper_bound(*it) ; ++it2){
	  const MFace &f = it2->second.second;
	  MVertex *v = it2->second.first;
	  MPyramid p (f.getVertex(0), f.getVertex(1), f.getVertex(2), f.getVertex(3), v);
	  minQual = std::min(minQual, fabs(p.minSICNShapeMeasure()));
	  std::vector<MElement*> e = adj[v];
	  e.push_back(&p);
	  v->setEntity (region);
	  double relax = std::min((double)(iter+1)/niter, 1.0);
	  //	  printf("%g (%d) --> ",e.size(),p.minSICNShapeMeasure());
	  _relocateVertexGolden( v, e, relax);
	  minQualOpti = std::min(minQualOpti, fabs(p.minSICNShapeMeasure()));
	  //	  printf("%g \n",p.minSICNShapeMeasure());
	  v->setEntity (*it);
	}
      }
    }
    //printf("relocation improves %g --> %g\n", minQual, minQualOpti);
  }
  int buildPyramids(GModel *gm)
  {
    if(empty()) return 0;
    int NBPY = 0;
    for (GModel::fiter it = gm->firstFace(); it != gm->lastFace(); ++it){
      std::set<MFace, Less_Face> allFaces;
      for (unsigned int i = 0; i < (*it)->triangles.size(); i++){
        allFaces.insert ((*it)->triangles[i]->getFace(0));
        delete (*it)->triangles[i];
      }
      (*it)->triangles.clear();
      for (std::multimap<GEntity*, std::pair<MVertex*,MFace> >::iterator it2 =
             _data.lower_bound(*it); it2 != _data.upper_bound(*it) ; ++it2){
        const MFace &f = it2->second.second;
        MVertex *v = it2->second.first;
        v->onWhat()->mesh_vertices.erase(std::find(v->onWhat()->mesh_vertices.begin(),
                                                   v->onWhat()->mesh_vertices.end(), v));
        std::set<MFace, Less_Face>::iterator itf0 = allFaces.find(MFace(f.getVertex(0),
                                                                        f.getVertex(1),v));
        std::set<MFace, Less_Face>::iterator itf1 = allFaces.find(MFace(f.getVertex(1),
                                                                        f.getVertex(2),v));
        std::set<MFace, Less_Face>::iterator itf2 = allFaces.find(MFace(f.getVertex(2),
                                                                        f.getVertex(3),v));
        std::set<MFace, Less_Face>::iterator itf3 = allFaces.find(MFace(f.getVertex(3),
                                                                        f.getVertex(0),v));
        if (itf0 != allFaces.end() && itf1 != allFaces.end() &&
            itf2 != allFaces.end() && itf3 != allFaces.end()){
          (*it)->quadrangles.push_back(new MQuadrangle(f.getVertex(0), f.getVertex(1),
                                                       f.getVertex(2), f.getVertex(3)));
          allFaces.erase(*itf0);
          allFaces.erase(*itf1);
          allFaces.erase(*itf2);
          allFaces.erase(*itf3);
          // printf("some pyramids should be created %d regions\n", (*it)->numRegions());
          for (int iReg = 0; iReg < (*it)->numRegions(); iReg++){
            if (iReg == 1) {
              Msg::Error("Cannot build pyramids on non manifold faces");
              v = new MVertex(v->x(), v->y(), v->z(), (*it)->getRegion(iReg));
            }
            else
              v->setEntity ((*it)->getRegion(iReg));
            // A quad face connected to an hex or a primsm --> leave the quad face as is
            if (_toDelete.find(f) == _toDelete.end()){
              (*it)->getRegion(iReg)->pyramids.push_back
                (new MPyramid(f.getVertex(0), f.getVertex(1), f.getVertex(2), f.getVertex(3), v));
              (*it)->getRegion(iReg)->mesh_vertices.push_back(v);
              NBPY++;
            }
            else {
              delete v;
            }
          }
        }
      }
      for (std::set<MFace, Less_Face>::iterator itf = allFaces.begin();
           itf != allFaces.end(); ++itf){
        (*it)->triangles.push_back
          (new MTriangle(itf->getVertex(0), itf->getVertex(1), itf->getVertex(2)));
      }
    }
    return NBPY;
  }
};

void getBoundingInfoAndSplitQuads(GRegion *gr,
                                  std::map<MFace,GEntity*,Less_Face> &allBoundingFaces,
                                  std::set<MVertex*> &allBoundingVertices,
                                  splitQuadRecovery &sqr)
{
  std::map<MFace, GEntity*, Less_Face> allBoundingFaces_temp;

  // Get all the faces that are on the boundary
  std::list<GFace*> faces = gr->faces();
  std::list<GFace*>::iterator it = faces.begin();
  while (it != faces.end()){
    GFace *gf = (*it);
    for(unsigned int i = 0; i < gf->getNumMeshElements(); i++){
      allBoundingFaces_temp[gf->getMeshElement(i)->getFace(0)] = gf;
    }
    ++it;
  }

  // if some elements pre-exist in the mesh, then use the internal faces of
  // those

  for (unsigned int i=0;i<gr->getNumMeshElements();i++){
    MElement *e = gr->getMeshElement(i);
    for (int j = 0; j < e->getNumFaces(); j++){
      std::map<MFace, GEntity*, Less_Face>::iterator it =
        allBoundingFaces_temp.find(e->getFace(j));
      if (it == allBoundingFaces_temp.end()) allBoundingFaces_temp[e->getFace(j)] = gr;
      else allBoundingFaces_temp.erase(it);
    }
  }

  std::map<MFace, GEntity*, Less_Face>::iterator itx = allBoundingFaces_temp.begin();
  for (; itx != allBoundingFaces_temp.end();++itx){
    const MFace &f = itx->first;
    // split the quad face into 4 triangular faces
    if (f.getNumVertices() == 4){
      sqr.setEmpty(false);
      MVertex *v0 = f.getVertex(0);
      MVertex *v1 = f.getVertex(1);
      MVertex *v2 = f.getVertex(2);
      MVertex *v3 = f.getVertex(3);
      MVertex *newv = new MVertex((v0->x() + v1->x() + v2->x() + v3->x())*0.25,
                                  (v0->y() + v1->y() + v2->y() + v3->y())*0.25,
                                  (v0->z() + v1->z() + v2->z() + v3->z())*0.25,
                                  itx->second);
      sqr.add(f,newv,itx->second);
      sqr._invmap[f] = newv;
      allBoundingFaces[MFace(v0,v1,newv)] = itx->second;
      allBoundingFaces[MFace(v1,v2,newv)] = itx->second;
      allBoundingFaces[MFace(v2,v3,newv)] = itx->second;
      allBoundingFaces[MFace(v3,v0,newv)] = itx->second;
      itx->second->mesh_vertices.push_back(newv);
      allBoundingVertices.insert(v0);
      allBoundingVertices.insert(v1);
      allBoundingVertices.insert(v2);
      allBoundingVertices.insert(v3);
      allBoundingVertices.insert(newv);
    }
    else{
      allBoundingFaces[f] = itx->second;
      allBoundingVertices.insert(f.getVertex(0));
      allBoundingVertices.insert(f.getVertex(1));
      allBoundingVertices.insert(f.getVertex(2));
    }
  }
}

#if defined(HAVE_TETGEN)

#include "tetgen.h"

void buildTetgenStructure(GRegion *gr, tetgenio &in, std::vector<MVertex*> &numberedV,
                          splitQuadRecovery &sqr)
{
  std::set<MVertex*> allBoundingVertices;
  std::map<MFace,GEntity*,Less_Face> allBoundingFaces;
  std::map<MEdge,GEntity*,Less_Edge> allBoundingEdges;

  // embedded edges
  {
    std::list<GEdge*> embe = gr->embeddedEdges();
    for (std::list<GEdge*>::iterator it = embe.begin(); it != embe.end(); ++it){
      for (unsigned int i = 0; i < (*it)->lines.size(); i++){
	MEdge me ((*it)->lines[i]->getVertex(0),(*it)->lines[i]->getVertex(1));
	allBoundingEdges[me] = *it;
	allBoundingVertices.insert((*it)->lines[i]->getVertex(0));
	allBoundingVertices.insert((*it)->lines[i]->getVertex(1));
      }
    }
  }

  // embedded vertices
  {
    std::list<GVertex*> embv = gr->embeddedVertices();
    for (std::list<GVertex*>::iterator it = embv.begin(); it != embv.end(); ++it){
      for (unsigned int i = 0; i < (*it)->points.size(); i++){
	allBoundingVertices.insert((*it)->points[i]->getVertex(0));
      }
    }
  }

  getBoundingInfoAndSplitQuads(gr, allBoundingFaces, allBoundingVertices, sqr);

  //// TEST
  {
    //    std::vector<MVertex*>ALL;
    //    std::vector<MTetrahedron*> MESH;
    //    ALL.insert(ALL.begin(),allBoundingVertices.begin(),allBoundingVertices.end());
    //    delaunayMeshIn3D (ALL,MESH);
    //    exit(1);
  }

  in.mesh_dim = 3;
  in.firstnumber = 1;
  int nbvertices_filler = (old_algo_hexa()) ? Filler::get_nbr_new_vertices() : Filler3D::get_nbr_new_vertices();
  in.numberofpoints = allBoundingVertices.size() + nbvertices_filler +
    LpSmoother::get_nbr_interior_vertices();
  in.pointlist = new REAL[in.numberofpoints * 3];
  in.pointmarkerlist = NULL;

  //printf("nbvertices_filler =%d\n",nbvertices_filler);

  std::set<MVertex*>::iterator itv = allBoundingVertices.begin();
  int I = 1;
  while(itv != allBoundingVertices.end()){
    in.pointlist[(I - 1) * 3 + 0] = (*itv)->x();
    in.pointlist[(I - 1) * 3 + 1] = (*itv)->y();
    in.pointlist[(I - 1) * 3 + 2] = (*itv)->z();
    (*itv)->setIndex(I++);
    numberedV.push_back(*itv);
    ++itv;
  }

  for(int i=0;i<nbvertices_filler;i++){
    MVertex* v;
    if (old_algo_hexa())
      v = Filler::get_new_vertex(i);
    else
      v = Filler3D::get_new_vertex(i);
    in.pointlist[(I - 1) * 3 + 0] = v->x();
    in.pointlist[(I - 1) * 3 + 1] = v->y();
    in.pointlist[(I - 1) * 3 + 2] = v->z();
    I++;
  }

  for(int i=0;i<LpSmoother::get_nbr_interior_vertices();i++){
    MVertex* v;
    v = LpSmoother::get_interior_vertex(i);
    in.pointlist[(I - 1) * 3 + 0] = v->x();
    in.pointlist[(I - 1) * 3 + 1] = v->y();
    in.pointlist[(I - 1) * 3 + 2] = v->z();
    I++;
  }


  in.numberofedges = allBoundingEdges.size();
  in.edgelist = new int[2*in.numberofedges];
  in.edgemarkerlist = new int[in.numberofedges];

  I = 0;
  std::map<MEdge,GEntity*,Less_Edge>::iterator ite = allBoundingEdges.begin();
  for (; ite != allBoundingEdges.end();++ite){
    const MEdge &ed = ite->first;
    in.edgelist[2*I]   = ed.getVertex(0)->getIndex();
    in.edgelist[2*I+1] = ed.getVertex(1)->getIndex();
    in.edgemarkerlist[I] = ite->second->tag();
    ++I;
  }

  in.numberoffacets = allBoundingFaces.size();
  in.facetlist = new tetgenio::facet[in.numberoffacets];
  in.facetmarkerlist = new int[in.numberoffacets];

  I = 0;
  std::map<MFace,GEntity*,Less_Face>::iterator it = allBoundingFaces.begin();
  for (; it != allBoundingFaces.end();++it){
    const MFace &fac = it->first;
    tetgenio::facet *f = &in.facetlist[I];
    tetgenio::init(f);
    f->numberofholes = 0;
    f->numberofpolygons = 1;
    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
    tetgenio::polygon *p = &f->polygonlist[0];
    tetgenio::init(p);
    p->numberofvertices = 3;
    p->vertexlist = new int[p->numberofvertices];
    p->vertexlist[0] = fac.getVertex(0)->getIndex();
    p->vertexlist[1] = fac.getVertex(1)->getIndex();
    p->vertexlist[2] = fac.getVertex(2)->getIndex();
    in.facetmarkerlist[I] = (it->second->dim() == 3) ? -it->second->tag() : it->second->tag();
    ++I;
  }
}

void TransferTetgenMesh(GRegion *gr, tetgenio &in, tetgenio &out,
                        std::vector<MVertex*> &numberedV)
{
  // improvement has to be done here : tetgen splits some of the existing edges
  // of the mesh. If those edges are classified on some model faces, new points
  // SHOULD be classified on the model face and get the right set of parametric
  // coordinates.

  const int initialSize = (int)numberedV.size();

  for(int i = numberedV.size(); i < out.numberofpoints; i++){
    MVertex *v = new MVertex(out.pointlist[i * 3 + 0],
                             out.pointlist[i * 3 + 1],
                             out.pointlist[i * 3 + 2], gr);
    gr->mesh_vertices.push_back(v);
    numberedV.push_back(v);
  }

  Msg::Info("%d points %d edges and %d faces in the initial mesh",
            out.numberofpoints, out.numberofedges, out.numberoftrifaces);

  // Tetgen modifies both surface & edge mesh, so we need to re-create
  // everything

  gr->model()->destroyMeshCaches();
  std::list<GFace*> faces = gr->faces();
  for(std::list<GFace*>::iterator it = faces.begin(); it != faces.end(); it++){
    GFace *gf = (*it);
    for(unsigned int i = 0; i < gf->triangles.size(); i++)
      delete gf->triangles[i];
    for(unsigned int i = 0; i < gf->quadrangles.size(); i++)
      delete gf->quadrangles[i];
    gf->triangles.clear();
    gf->quadrangles.clear();
    gf->deleteVertexArrays();
  }

  // TODO: re-create 1D mesh
  /*for(int i = 0; i < out.numberofedges; i++){
  MVertex *v[2];
  v[0] = numberedV[out.edgelist[i * 2 + 0] - 1];
  v[1] = numberedV[out.edgelist[i * 2 + 1] - 1];
  //implement here the 1D mesh ...
  }*/

  bool needParam = (CTX::instance()->mesh.order > 1 &&
                    CTX::instance()->mesh.secondOrderExperimental);
  // re-create the triangular meshes FIXME: this can lead to hanging nodes for
  // non manifold geometries (single surface connected to volume)
  for(int i = 0; i < out.numberoftrifaces; i++){
    //    printf("%d %d %d\n",i,out.numberoftrifaces,needParam);

    if (out.trifacemarkerlist[i] > 0) {
      MVertex *v[3];
      v[0] = numberedV[out.trifacelist[i * 3 + 0] - 1];
      v[1] = numberedV[out.trifacelist[i * 3 + 1] - 1];
      v[2] = numberedV[out.trifacelist[i * 3 + 2] - 1];
      // do not recover prismatic faces !!!
      GFace *gf = gr->model()->getFaceByTag(out.trifacemarkerlist[i]);

      double guess[2] = {0, 0};
      if (needParam) {
        int Count = 0;
        for(int j = 0; j < 3; j++){
          if(!v[j]->onWhat()){
            Msg::Error("Uncategorized vertex %d", v[j]->getNum());
          }
          else if(v[j]->onWhat()->dim() == 2){
            double uu,vv;
            v[j]->getParameter(0, uu);
            v[j]->getParameter(1, vv);
            guess[0] += uu;
            guess[1] += vv;
            Count++;
          }
          else if(v[j]->onWhat()->dim() == 1){
            GEdge *ge = (GEdge*)v[j]->onWhat();
            double UU;
            v[j]->getParameter(0, UU);
            SPoint2 param;
            param = ge->reparamOnFace(gf, UU, 1);
            guess[0] += param.x();
            guess[1] += param.y();
            Count++;
          }
          else if(v[j]->onWhat()->dim() == 0){
            SPoint2 param;
            GVertex *gv = (GVertex*)v[j]->onWhat();
            param = gv->reparamOnFace(gf,1);
            guess[0] += param.x();
            guess[1] += param.y();
            Count++;
          }
        }
        if(Count != 0){
          guess[0] /= Count;
          guess[1] /= Count;
        }
      }

      for(int j = 0; j < 3; j++){
        if (out.trifacelist[i * 3 + j] - 1 >= initialSize){
          //        if(v[j]->onWhat()->dim() == 3){
          v[j]->onWhat()->mesh_vertices.erase
            (std::find(v[j]->onWhat()->mesh_vertices.begin(),
                       v[j]->onWhat()->mesh_vertices.end(),
                       v[j]));
          MVertex *v1b;
          if(needParam){
            // parametric coordinates should be set for the vertex !  (this is
            // not 100 % safe yet, so we reserve that operation for high order
            // meshes)
            GPoint gp = gf->closestPoint(SPoint3(v[j]->x(), v[j]->y(), v[j]->z()), guess);
            if(gp.g()){
              v1b = new MFaceVertex(gp.x(), gp.y(), gp.z(), gf, gp.u(), gp.v());
            }
            else{
              v1b = new MVertex(v[j]->x(), v[j]->y(), v[j]->z(), gf);
              Msg::Warning("The point was not projected back to the surface (%g %g %g)",
                           v[j]->x(), v[j]->y(), v[j]->z());
            }
          }
          else{
            v1b = new MVertex(v[j]->x(), v[j]->y(), v[j]->z(), gf);
          }

          gf->mesh_vertices.push_back(v1b);
          numberedV[out.trifacelist[i * 3 + j] - 1] = v1b;
          delete v[j];
          v[j] = v1b;
        }
      }
      //      printf("new triangle %d %d %d\n",v[0]->onWhat()->dim(), v[1]->onWhat()->dim(), v[2]->onWhat()->dim());
      MTriangle *t = new MTriangle(v[0], v[1], v[2]);
      gf->triangles.push_back(t);
    }// do not do anything with prismatic faces
  }


  for(int i = 0; i < out.numberoftetrahedra; i++){
    MVertex *v1 = numberedV[out.tetrahedronlist[i * 4 + 0] - 1];
    MVertex *v2 = numberedV[out.tetrahedronlist[i * 4 + 1] - 1];
    MVertex *v3 = numberedV[out.tetrahedronlist[i * 4 + 2] - 1];
    MVertex *v4 = numberedV[out.tetrahedronlist[i * 4 + 3] - 1];
    MTetrahedron *t = new  MTetrahedron(v1, v2, v3, v4);
    gr->tetrahedra.push_back(t);
  }
}

bool CreateAnEmptyVolumeMesh(GRegion *gr)
{
  //printf("creating an empty volume mesh\n");
  splitQuadRecovery sqr;
  tetgenio in, out;
  std::vector<MVertex*> numberedV;
  char opts[128];
  buildTetgenStructure(gr, in, numberedV, sqr);
  //printf("tetgen structure created\n");
  sprintf(opts, "-Ype%sT%g",
          (Msg::GetVerbosity() < 3) ? "Q" : (Msg::GetVerbosity() > 6) ? "V" : "",
          CTX::instance()->mesh.toleranceInitialDelaunay);
  try{
    tetrahedralize(opts, &in, &out);
  }
  catch (int error){
    Msg::Error("Self intersecting surface mesh");
    return false;
  }
  //printf("creating an empty volume mesh done\n");
  TransferTetgenMesh(gr, in, out, numberedV);
  return true;
}

void MeshDelaunayVolumeTetgen(std::vector<GRegion*> &regions)
{
  if(regions.empty()) return;

  for(unsigned int i = 0; i < regions.size(); i++)
    Msg::Info("Meshing volume %d (Delaunay)", regions[i]->tag());

  // put all the faces in the region
  GRegion *gr = regions[0];
  std::list<GFace*> faces = gr->faces();

  std::set<GFace*> allFacesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GFace*> f = regions[i]->faces();
    allFacesSet.insert(f.begin(), f.end());
    f = regions[i]->embeddedFaces();
    allFacesSet.insert(f.begin(), f.end());
  }
  std::list<GFace*> allFaces;
  for(std::set<GFace*>::iterator it = allFacesSet.begin(); it != allFacesSet.end(); it++){
    allFaces.push_back(*it);
  }
  gr->set(allFaces);

  // put all embedded edges in the same region
  std::set<GEdge*> allEmbEdgesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GEdge*> e = regions[i]->embeddedEdges();
    allEmbEdgesSet.insert(e.begin(), e.end());
  }
  std::list<GEdge*> allEmbEdges;
  allEmbEdges.insert(allEmbEdges.end(), allEmbEdgesSet.begin(), allEmbEdgesSet.end());
  std::list<GEdge*> oldEmbEdges = gr->embeddedEdges();
  gr->embeddedEdges() = allEmbEdges;

  // put all embedded vertices in the same region
  std::set<GVertex*> allEmbVerticesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GVertex*> e = regions[i]->embeddedVertices();
    allEmbVerticesSet.insert(e.begin(), e.end());
  }
  std::list<GVertex*> allEmbVertices;
  allEmbVertices.insert(allEmbVertices.end(), allEmbVerticesSet.begin(), allEmbVerticesSet.end());
  std::list<GVertex*> oldEmbVertices = gr->embeddedVertices();
  gr->embeddedVertices() = allEmbVertices;

  // mesh with tetgen, possibly changing the mesh on boundaries (leave
  // this in block, so in/out are destroyed before we refine the mesh)
  splitQuadRecovery sqr;
  {
    tetgenio in, out;
    std::vector<MVertex*> numberedV;
    char opts[128];
    buildTetgenStructure(gr, in, numberedV, sqr);
    sprintf(opts, "%sYpe%sT%g",
            CTX::instance()->mesh.algo3d == ALGO_3D_RTREE ? "S0" : "",
            (Msg::GetVerbosity() < 3) ? "Q" : (Msg::GetVerbosity() > 6) ? "V" : "",
            CTX::instance()->mesh.toleranceInitialDelaunay);
    try{
      tetrahedralize(opts, &in, &out);
    }
    catch (int error){
      Msg::Error("Self intersecting surface mesh, computing intersections "
                 "(this could take a while)");
      sprintf(opts, "dV");
      try{
        tetrahedralize(opts, &in, &out);
        Msg::Info("%d intersecting faces have been saved into 'intersect.pos'",
                  out.numberoftrifaces);
        FILE *fp = Fopen("intersect.pos", "w");
        if(fp){
          fprintf(fp, "View \"intersections\" {\n");
          for(int i = 0; i < out.numberoftrifaces; i++){
            MVertex *v1 = numberedV[out.trifacelist[i * 3 + 0] - 1];
            MVertex *v2 = numberedV[out.trifacelist[i * 3 + 1] - 1];
            MVertex *v3 = numberedV[out.trifacelist[i * 3 + 2] - 1];
            int surf = out.trifacemarkerlist[i];
            fprintf(fp, "ST(%g,%g,%g,%g,%g,%g,%g,%g,%g){%d,%d,%d};\n",
                    v1->x(), v1->y(), v1->z(), v2->x(), v2->y(), v2->z(),
                    v3->x(), v3->y(), v3->z(), surf, surf, surf);
          }
          fprintf(fp, "};\n");
          fclose(fp);
        }
        else
          Msg::Error("Could not open file 'intersect.pos'");
      }
      catch (int error2){
        Msg::Error("Surface mesh is wrong, cannot do the 3D mesh");
      }
      gr->set(faces);
      return;
    }
    TransferTetgenMesh(gr, in, out, numberedV);
  }


  // sort triangles in all model faces in order to be able to search in vectors
  std::list<GFace*>::iterator itf =  allFaces.begin();
  while(itf != allFaces.end()){
    compareMTriangleLexicographic cmp;
    std::sort((*itf)->triangles.begin(), (*itf)->triangles.end(), cmp);
    ++itf;
  }

  // restore the initial set of faces and embedded edges/vertices
  gr->set(faces);
  gr->embeddedEdges() = oldEmbEdges;
  gr->embeddedVertices() = oldEmbVertices;

  // FIXME: this screws up refinement of standard meshes
  //insertVerticesInRegion(gr,0,true);
  //sqr.relocateVertices(gr,3);

  // now do insertion of points
  if(CTX::instance()->mesh.algo3d == ALGO_3D_FRONTAL_DEL)
    Msg::Error ("Frontal Layers Algo deprecated");
  else if(CTX::instance()->mesh.algo3d == ALGO_3D_FRONTAL_HEX)
    Msg::Error ("Frontal Layers Algo deprecated");
  else if(CTX::instance()->mesh.algo3d == ALGO_3D_MMG3D){
    refineMeshMMG(gr);
  }
  else{
    int nbvertices_filler = (old_algo_hexa()) ?
      Filler::get_nbr_new_vertices() : Filler3D::get_nbr_new_vertices();
    if(!nbvertices_filler && !LpSmoother::get_nbr_interior_vertices()){
      insertVerticesInRegion(gr,2000000000,true);
    }
  }

  // crete an initial mesh
  if (sqr.buildPyramids (gr->model())){
    RelocateVertices(regions, 3);
  }
}

#else

bool CreateAnEmptyVolumeMesh(GRegion *gr)
{
  Msg::Error("You should compile with TETGEN in order to create an empty volume mesh");
  return false;
}

#endif

static void MeshDelaunayVolumeNewCode(std::vector<GRegion*> &regions)
{
  GRegion *gr = regions[0];
  std::list<GFace*> faces = gr->faces();
  std::set<GFace*, GEntityLessThan> allFacesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GFace*> f = regions[i]->faces();
    allFacesSet.insert(f.begin(), f.end());
    f = regions[i]->embeddedFaces();
    allFacesSet.insert(f.begin(), f.end());
  }
  std::list<GFace*> allFaces;
  allFaces.insert(allFaces.end(), allFacesSet.begin(), allFacesSet.end());
  gr->set(allFaces);

  std::set<GEdge*> allEmbEdgesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GEdge*> e = regions[i]->embeddedEdges();
    allEmbEdgesSet.insert(e.begin(), e.end());
  }
  std::list<GEdge*> allEmbEdges;
  allEmbEdges.insert(allEmbEdges.end(), allEmbEdgesSet.begin(), allEmbEdgesSet.end());
  std::list<GEdge*> oldEmbEdges = gr->embeddedEdges();
  gr->embeddedEdges() = allEmbEdges;

  std::set<GVertex*> allEmbVerticesSet;
  for(unsigned int i = 0; i < regions.size(); i++){
    std::list<GVertex*> e = regions[i]->embeddedVertices();
    allEmbVerticesSet.insert(e.begin(), e.end());
  }
  std::list<GVertex*> allEmbVertices;
  allEmbVertices.insert(allEmbVertices.end(), allEmbVerticesSet.begin(), allEmbVerticesSet.end());
  std::list<GVertex*> oldEmbVertices = gr->embeddedVertices();
  gr->embeddedVertices() = allEmbVertices;

  meshGRegionBoundaryRecovery(gr);
  /*
    FILE *fp = Fopen("debug.pos", "w");
    if(fp){
      fprintf(fp, "View \"debug\" {\n");
      for(unsigned int i = 0; i < gr->tetrahedra.size(); i++)
        gr->tetrahedra[i]->writePOS(fp, 1., gr->tag(),false,false,false,1,1);
      fprintf(fp, "};\n");
      fclose(fp);
    }
  */

  // sort triangles in all model faces in order to be able to search in vectors
  std::list<GFace*>::iterator itf =  allFaces.begin();
  while(itf != allFaces.end()){
    compareMTriangleLexicographic cmp;
    std::sort((*itf)->triangles.begin(), (*itf)->triangles.end(), cmp);
    ++itf;
  }

  // restore the initial set of faces and embedded edges/vertices
  gr->set(faces);
  gr->embeddedEdges() = oldEmbEdges;
  gr->embeddedVertices() = oldEmbVertices;

  // now do insertion of points
  if(CTX::instance()->mesh.oldRefinement){
    insertVerticesInRegion(gr, 2000000000, true);
  }
  else{
    void edgeBasedRefinement(const int numThreads,
			     const int nptsatonce,
			     GRegion *gr);
    // just to remove tets that are not to be meshed
    insertVerticesInRegion(gr, 0);
    for(unsigned int i = 0; i < regions.size(); i++){
      Msg::Info("Refining volume %d with %d threads",regions[i]->tag(),Msg::GetMaxThreads());
      edgeBasedRefinement(Msg::GetMaxThreads(), 1, regions[i]);
    }
    // RelocateVertices (regions,-1);
  }
}

void MeshDelaunayVolume(std::vector<GRegion*> &regions)
{
  if(regions.empty()) return;

  if(CTX::instance()->mesh.algo3d == ALGO_3D_DELAUNAY_NEW){
    MeshDelaunayVolumeNewCode(regions);
  }
  else{
#if defined(HAVE_TETGEN)
    MeshDelaunayVolumeTetgen(regions);
#else
    Msg::Error("Old Delaunay algorithm requires Tetgen");
#endif
  }
  return;
}

#if defined(HAVE_NETGEN)

namespace nglib {
#include "nglib_gmsh.h"
}
using namespace nglib;

static void getAllBoundingVertices(GRegion *gr,
                                   std::set<MVertex*, MVertexLessThanNum> &allBoundingVertices)
{
  std::list<GFace*> faces = gr->faces();
  std::list<GFace*>::iterator it = faces.begin();

  while (it != faces.end()){
    GFace *gf = (*it);
    for(unsigned int i = 0; i < gf->triangles.size(); i++){
      MTriangle *t = gf->triangles[i];
      for(int k = 0; k < 3; k++)
        if(allBoundingVertices.find(t->getVertex(k)) ==  allBoundingVertices.end())
          allBoundingVertices.insert(t->getVertex(k));
    }
    ++it;
  }
}

Ng_Mesh *buildNetgenStructure(GRegion *gr, bool importVolumeMesh,
                              std::vector<MVertex*> &numberedV)
{
  Ng_Init();
  Ng_Mesh *ngmesh = Ng_NewMesh();

  std::set<MVertex*, MVertexLessThanNum> allBoundingVertices;
  getAllBoundingVertices(gr, allBoundingVertices);

  std::set<MVertex*, MVertexLessThanNum>::iterator itv = allBoundingVertices.begin();
  int I = 1;
  while(itv != allBoundingVertices.end()){
    double tmp[3];
    tmp[0] = (*itv)->x();
    tmp[1] = (*itv)->y();
    tmp[2] = (*itv)->z();
    (*itv)->setIndex(I++);
    numberedV.push_back(*itv);
    Ng_AddPoint(ngmesh, tmp);
    ++itv;
  }

  if(importVolumeMesh){
    for(unsigned int i = 0; i < gr->mesh_vertices.size(); i++){
      double tmp[3];
      tmp[0] = gr->mesh_vertices[i]->x();
      tmp[1] = gr->mesh_vertices[i]->y();
      tmp[2] = gr->mesh_vertices[i]->z();
      gr->mesh_vertices[i]->setIndex(I++);
      Ng_AddPoint(ngmesh, tmp);
    }
  }
  std::list<GFace*> faces = gr->faces();
  std::list<GFace*>::iterator it = faces.begin();
  while(it != faces.end()){
    GFace *gf = (*it);
    for(unsigned int i = 0; i< gf->triangles.size(); i++){
      MTriangle *t = gf->triangles[i];
      int tmp[3];
      tmp[0] = t->getVertex(0)->getIndex();
      tmp[1] = t->getVertex(1)->getIndex();
      tmp[2] = t->getVertex(2)->getIndex();
      Ng_AddSurfaceElement(ngmesh, NG_TRIG, tmp);
    }
    ++it;
  }

  if(importVolumeMesh){
    for(unsigned int i = 0; i< gr->tetrahedra.size(); i++){
      MTetrahedron *t = gr->tetrahedra[i];
      // netgen expects tet with negative volume
      if(t->getVolumeSign() > 0) t->reverse();
      int tmp[4];
      tmp[0] = t->getVertex(0)->getIndex();
      tmp[1] = t->getVertex(1)->getIndex();
      tmp[2] = t->getVertex(2)->getIndex();
      tmp[3] = t->getVertex(3)->getIndex();
      Ng_AddVolumeElement(ngmesh, NG_TET, tmp);
    }
  }

  return ngmesh;
}

void TransferVolumeMesh(GRegion *gr, Ng_Mesh *ngmesh,
                        std::vector<MVertex*> &numberedV)
{
  // Gets total number of vertices of Netgen's mesh
  int nbv = Ng_GetNP(ngmesh);
  if(!nbv) return;

  int nbpts = numberedV.size();

  // Create new volume vertices
  for(int i = nbpts; i < nbv; i++){
    double tmp[3];
    Ng_GetPoint(ngmesh, i + 1, tmp);
    MVertex *v = new MVertex(tmp[0], tmp[1], tmp[2], gr);
    numberedV.push_back(v);
    gr->mesh_vertices.push_back(v);
  }

  // Get total number of simplices of Netgen's mesh
  int nbe = Ng_GetNE(ngmesh);

  // Create new volume simplices
  for(int i = 0; i < nbe; i++){
    int tmp[4];
    Ng_GetVolumeElement(ngmesh, i + 1, tmp);
    MTetrahedron *t = new MTetrahedron(numberedV[tmp[0] - 1],
                                       numberedV[tmp[1] - 1],
                                       numberedV[tmp[2] - 1],
                                       numberedV[tmp[3] - 1]);
    gr->tetrahedra.push_back(t);
  }
}

#endif

void deMeshGRegion::operator() (GRegion *gr)
{
  if(gr->geomType() == GEntity::DiscreteVolume) return;
  gr->deleteMesh();
}

/// X_1 (1-u-v) + X_2 u + X_3 v = P_x + t N_x
/// Y_1 (1-u-v) + Y_2 u + Y_3 v = P_y + t N_y
/// Z_1 (1-u-v) + Z_2 u + Z_3 v = P_z + t N_z

int intersect_line_triangle(double X[3], double Y[3], double Z[3] ,
                            double P[3], double N[3], const double eps_prec)
{
  double mat[3][3], det;
  double b[3], res[3];

  mat[0][0] = X[1] - X[0];
  mat[0][1] = X[2] - X[0];
  mat[0][2] = N[0];

  mat[1][0] = Y[1] - Y[0];
  mat[1][1] = Y[2] - Y[0];
  mat[1][2] = N[1];

  mat[2][0] = Z[1] - Z[0];
  mat[2][1] = Z[2] - Z[0];
  mat[2][2] = N[2];

  b[0] = P[0] - X[0];
  b[1] = P[1] - Y[0];
  b[2] = P[2] - Z[0];

  if(!sys3x3_with_tol(mat, b, res, &det)){
    return 0;
  }
  //  printf("coucou %g %g %g\n",res[0],res[1],res[2]);
  if(res[0] >= eps_prec && res[0] <= 1.0 - eps_prec &&
     res[1] >= eps_prec && res[1] <= 1.0 - eps_prec &&
     1 - res[0] - res[1] >= eps_prec && 1 - res[0] - res[1] <= 1.0 - eps_prec){
    // the line clearly intersects the triangle
    return (res[2] > 0) ? 1 : 0;
  }
  else if(res[0] < -eps_prec || res[0] > 1.0 + eps_prec ||
          res[1] < -eps_prec || res[1] > 1.0 + eps_prec ||
          1 - res[0] - res[1] < -eps_prec || 1 - res[0] - res[1] > 1.0 + eps_prec){
    // the line clearly does NOT intersect the triangle
    return 0;
  }
  else{
    //printf("non robust stuff\n");
    // the intersection is not robust, try another triangle
    return -10000;
  }
}

void setRand(double r[6])
{
  for(int i = 0; i < 6; i++)
    r[i] = 0.0001 * ((double)rand() / (double)RAND_MAX);
}

void meshNormalsPointOutOfTheRegion(GRegion *gr)
{
  std::list<GFace*> faces = gr->faces();
  std::list<GFace*>::iterator it = faces.begin();

  // perform intersection check in normalized coordinates
  SBoundingBox3d bbox = gr->bounds();
  double scaling = norm(SVector3(bbox.max(), bbox.min()));
  if(!scaling){
    Msg::Warning("Bad scaling in meshNormalsPointOutOfTheRegion");
    scaling = 1.;
  }

  double rrr[6];
  setRand(rrr);

  while(it != faces.end()){
    GFace *gf = (*it);
    int nb_intersect = 0;
    for(unsigned int i = 0; i < gf->triangles.size(); i++){
      MTriangle *t = gf->triangles[i];
      double X[3] = {t->getVertex(0)->x(), t->getVertex(1)->x(), t->getVertex(2)->x()};
      double Y[3] = {t->getVertex(0)->y(), t->getVertex(1)->y(), t->getVertex(2)->y()};
      double Z[3] = {t->getVertex(0)->z(), t->getVertex(1)->z(), t->getVertex(2)->z()};
      for(int i = 0; i < 3; i++){
        X[i] /= scaling;
        Y[i] /= scaling;
        Z[i] /= scaling;
      }

      double P[3] = {(X[0] + X[1] + X[2]) / 3.,
                     (Y[0] + Y[1] + Y[2]) / 3.,
                     (Z[0] + Z[1] + Z[2]) / 3.};
      double v1[3] = {X[0] - X[1], Y[0] - Y[1], Z[0] - Z[1]};
      double v2[3] = {X[2] - X[1], Y[2] - Y[1], Z[2] - Z[1]};
      double N[3];
      prodve(v1, v2, N);
      norme(v1);
      norme(v2);
      norme(N);
      N[0] += rrr[0] * v1[0] + rrr[1] * v2[0];
      N[1] += rrr[2] * v1[1] + rrr[3] * v2[1];
      N[2] += rrr[4] * v1[2] + rrr[5] * v2[2];
      norme(N);
      std::list<GFace*>::iterator it_b = faces.begin();
      while(it_b != faces.end()){
        GFace *gf_b = (*it_b);
        for(unsigned int i_b = 0; i_b < gf_b->triangles.size(); i_b++){
          MTriangle *t_b = gf_b->triangles[i_b];
          if(t_b != t){
            double X_b[3] = {t_b->getVertex(0)->x(), t_b->getVertex(1)->x(),
                             t_b->getVertex(2)->x()};
            double Y_b[3] = {t_b->getVertex(0)->y(), t_b->getVertex(1)->y(),
                             t_b->getVertex(2)->y()};
            double Z_b[3] = {t_b->getVertex(0)->z(), t_b->getVertex(1)->z(),
                             t_b->getVertex(2)->z()};
            for(int i = 0; i < 3; i++){
              X_b[i] /= scaling;
              Y_b[i] /= scaling;
              Z_b[i] /= scaling;
            }
            int inters = intersect_line_triangle(X_b, Y_b, Z_b, P, N, 1.e-9);
            nb_intersect += inters;
          }
        }
        ++it_b;
      }
      Msg::Info("Region %d Face %d, %d intersect", gr->tag(), gf->tag(), nb_intersect);
      if(nb_intersect >= 0) break; // negative value means intersection is not "robust"
    }

    if(nb_intersect < 0){
      setRand(rrr);
    }
    else{
      if(nb_intersect % 2 == 1){
        // odd nb of intersections: the normal points inside the region
        for(unsigned int i = 0; i < gf->triangles.size(); i++){
          gf->triangles[i]->reverse();
        }
      }
      ++it;
    }
  }

  // FILE *fp = Fopen("debug.pos", "w");
  // if(fp){
  //   fprintf(fp, "View \"debug\" {\n");
  //   for(std::list<GFace*>::iterator it = faces.begin(); it != faces.end(); it++)
  //     for(unsigned int i = 0; i < (*it)->triangles.size(); i++)
  //       (*it)->triangles[i]->writePOS(fp, 1., (*it)->tag());
  //   fprintf(fp, "};\n");
  //   fclose(fp);
  // }
}

void meshGRegion::operator() (GRegion *gr)
{

  gr->model()->setCurrentMeshEntity(gr);

  if(gr->geomType() == GEntity::DiscreteVolume) return;
  if(gr->meshAttributes.method == MESH_NONE) return;
  if(CTX::instance()->mesh.meshOnlyVisible && !gr->getVisibility()) return;

  ExtrudeParams *ep = gr->meshAttributes.extrude;
  if(ep && ep->mesh.ExtrudeMesh) return;

  // destroy the mesh if it exists
  deMeshGRegion dem;
  dem(gr);

  if(MeshTransfiniteVolume(gr)) return;

  std::list<GFace*> faces = gr->faces();

  // sanity check for frontal algo
  if(CTX::instance()->mesh.algo3d == ALGO_3D_FRONTAL){
    for(std::list<GFace*>::iterator it = faces.begin(); it != faces.end(); it++){
      if((*it)->quadrangles.size()){
        Msg::Error("Cannot use frontal 3D algorithm with quadrangles on boundary");
        return;
      }
    }
  }

  // replace discreteFaces by their compounds
  {
    std::set<GFace*, GEntityLessThan> mySet;
    std::list<GFace*>::iterator it = faces.begin();
    while(it != faces.end()){
      if((*it)->getCompound())
        mySet.insert((*it)->getCompound());
      else
        mySet.insert(*it);
      ++it;
    }
    faces.clear();
    faces.insert(faces.begin(), mySet.begin(), mySet.end());
    gr->set(faces);
  }

  if(CTX::instance()->mesh.algo3d != ALGO_3D_FRONTAL){
    delaunay.push_back(gr);
  }
  else if(CTX::instance()->mesh.algo3d == ALGO_3D_FRONTAL){
#if !defined(HAVE_NETGEN)
    Msg::Error("Frontal algorithm requires Netgen");
#else
    Msg::Info("Meshing volume %d (Frontal)", gr->tag());
    // orient the triangles of with respect to this region
    meshNormalsPointOutOfTheRegion(gr);
    std::vector<MVertex*> numberedV;
    Ng_Mesh *ngmesh = buildNetgenStructure(gr, false, numberedV);
    Ng_GenerateVolumeMesh(ngmesh, CTX::instance()->mesh.lcMax);
    TransferVolumeMesh(gr, ngmesh, numberedV);
    Ng_DeleteMesh(ngmesh);
    Ng_Exit();
#endif
  }

}

void optimizeMeshGRegionNetgen::operator() (GRegion *gr, bool always)
{
  gr->model()->setCurrentMeshEntity(gr);

  if(!always && gr->geomType() == GEntity::DiscreteVolume) return;

  // don't optimize transfinite or extruded meshes
  if(gr->meshAttributes.method == MESH_TRANSFINITE) return;
  ExtrudeParams *ep = gr->meshAttributes.extrude;
  if(ep && ep->mesh.ExtrudeMesh && ep->geo.Mode == EXTRUDED_ENTITY) return;

#if !defined(HAVE_NETGEN)
  Msg::Error("Netgen optimizer is not compiled in this version of Gmsh");
#else
  Msg::Info("Optimizing volume %d", gr->tag());
  // import mesh into netgen, including volume tets
  std::vector<MVertex*> numberedV;
  Ng_Mesh *ngmesh = buildNetgenStructure(gr, true, numberedV);
  // delete volume vertices and tets
  deMeshGRegion dem;
  dem(gr);
  // optimize mesh
  Ng_OptimizeVolumeMesh(ngmesh, CTX::instance()->mesh.lcMax);
  TransferVolumeMesh(gr, ngmesh, numberedV);
  Ng_DeleteMesh(ngmesh);
  Ng_Exit();
#endif
}

void optimizeMeshGRegionGmsh::operator() (GRegion *gr, bool always)
{
  gr->model()->setCurrentMeshEntity(gr);

  if(!always && gr->geomType() == GEntity::DiscreteVolume) return;

  // don't optimize extruded meshes
  if(gr->meshAttributes.method == MESH_TRANSFINITE) return;
  ExtrudeParams *ep = gr->meshAttributes.extrude;
  if(ep && ep->mesh.ExtrudeMesh && ep->geo.Mode == EXTRUDED_ENTITY) return;

  Msg::Info("Optimizing volume %d", gr->tag());
  optimizeMesh(gr, qmTetrahedron::QMTET_GAMMA);
}


bool buildFaceSearchStructure(GModel *model, fs_cont &search)
{
  search.clear();

  std::set<GFace*> faces_to_consider;
  GModel::riter rit = model->firstRegion();
  while(rit != model->lastRegion()){
    std::list <GFace *> _faces = (*rit)->faces();
    faces_to_consider.insert( _faces.begin(),_faces.end());
    rit++;
  }

  std::set<GFace*>::iterator fit = faces_to_consider.begin();
  while(fit != faces_to_consider.end()){
    for(unsigned int i = 0; i < (*fit)->getNumMeshElements(); i++){
      MFace ff = (*fit)->getMeshElement(i)->getFace(0);
      search[ff] = *fit;
    }
    ++fit;
  }
  return true;
}

bool buildEdgeSearchStructure(GModel *model, es_cont &search)
{
  search.clear();

  GModel::eiter eit = model->firstEdge();
  while(eit != model->lastEdge()){
    for(unsigned int i = 0; i < (*eit)->lines.size(); i++){
      MVertex *p1 = (*eit)->lines[i]->getVertex(0);
      MVertex *p2 = (*eit)->lines[i]->getVertex(1);
      MVertex *p = std::min(p1, p2);
      search.insert(std::pair<MVertex*, std::pair<MLine*, GEdge*> >
                    (p, std::pair<MLine*, GEdge*>((*eit)->lines[i], *eit)));
    }
    ++eit;
  }
  return true;
}

GFace *findInFaceSearchStructure(MVertex *p1, MVertex *p2, MVertex *p3,
                                 const fs_cont &search)
{
  MFace ff(p1,p2,p3);
  fs_cont::const_iterator it = search.find(ff);
  if (it == search.end())return 0;
  return it->second;
}

GFace *findInFaceSearchStructure(const MFace &ff,
                                 const fs_cont &search)
{
  fs_cont::const_iterator it = search.find(ff);
  if (it == search.end())return 0;
  return it->second;
}


GEdge *findInEdgeSearchStructure(MVertex *p1, MVertex *p2, const es_cont &search)
{
  MVertex *p = std::min(p1, p2);

  for(es_cont::const_iterator it = search.lower_bound(p);
      it != search.upper_bound(p);
      ++it){
    MLine *l = it->second.first;
    GEdge *ge = it->second.second;
    if((l->getVertex(0) == p1 || l->getVertex(0) == p2) &&
       (l->getVertex(1) == p1 || l->getVertex(1) == p2))
      return ge;
  }
  return 0;
}
