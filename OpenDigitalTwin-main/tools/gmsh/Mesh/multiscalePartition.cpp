// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.
//
// Contributor(s):
//   Emilie Marchandise

#include "multiscalePartition.h"
#include "GmshConfig.h"
#include "GmshDefines.h"
#include "meshPartition.h"
#include "MEdge.h"
#include "MElement.h"
#include "multiscaleLaplace.h"
#include "GFaceCompound.h"
#include "Numeric.h"
#include "Context.h"
#include "OS.h"

static void recur_connect(MVertex *v,
                          std::multimap<MVertex*,MEdge> &v2e,
                          std::set<MEdge,Less_Edge> &group,
                          std::set<MVertex*> &touched)
{
  if (touched.find(v) != touched.end())return;

  touched.insert(v);
  for (std::multimap <MVertex*,MEdge>::iterator it = v2e.lower_bound(v);
       it != v2e.upper_bound(v) ; ++it){
    group.insert(it->second);
    for (int i=0;i<it->second.getNumVertices();++i){
      recur_connect (it->second.getVertex(i),v2e,group,touched);
    }
  }

}


// starting form a list of elements, returns
// lists of lists that are all simply connected
static void recur_connect_e (const MEdge &e,
                             std::multimap<MEdge,MElement*,Less_Edge> &e2e,
                             std::set<MElement*> &group,
                             std::set<MEdge,Less_Edge> &touched){
  if (touched.find(e) != touched.end())return;
  touched.insert(e);
  for (std::multimap <MEdge,MElement*,Less_Edge>::iterator it = e2e.lower_bound(e);
         it != e2e.upper_bound(e) ; ++it){
    group.insert(it->second);
    for (int i=0;i<it->second->getNumEdges();++i){
      recur_connect_e (it->second->getEdge(i),e2e,group,touched);
    }
  }
}


static int connected_bounds (std::vector<MEdge> &edges,  std::vector<std::vector<MEdge> > &boundaries)
{
  std::multimap<MVertex*,MEdge> v2e;
  for (unsigned i = 0; i < edges.size(); ++i){
    for (int j=0;j<edges[i].getNumVertices();j++){
      v2e.insert(std::make_pair(edges[i].getVertex(j),edges[i]));
    }
  }
  while (!v2e.empty()){
    std::set<MEdge, Less_Edge> group;
    std::set<MVertex*> touched;
    recur_connect (v2e.begin()->first,v2e,group,touched);
    std::vector<MEdge> temp;
    temp.insert(temp.begin(), group.begin(), group.end());
    boundaries.push_back(temp);
    for (std::set<MVertex*>::iterator it = touched.begin() ; it != touched.end();++it)
      v2e.erase(*it);
  }

  return boundaries.size();
}

//--------------------------------------------------------------
static void connectedRegions (std::vector<MElement*> &elements,
                              std::vector<std::vector<MElement*> > &regions)
{
  std::multimap<MEdge,MElement*,Less_Edge> e2e;
  for (unsigned int i = 0; i < elements.size(); ++i){
    for (int j = 0; j < elements[i]->getNumEdges(); j++){
      e2e.insert(std::make_pair(elements[i]->getEdge(j),elements[i]));
    }
  }
  while (!e2e.empty()){
    std::set<MElement*> group;
    std::set<MEdge,Less_Edge> touched;
    recur_connect_e (e2e.begin()->first,e2e,group,touched);
    std::vector<MElement*> temp;
    temp.insert(temp.begin(), group.begin(), group.end());
    regions.push_back(temp);
    for ( std::set<MEdge,Less_Edge>::iterator it = touched.begin() ; it != touched.end();++it)
      e2e.erase(*it);
  }
}

static int getGenus (std::vector<MElement *> &elements,
                     std::vector<std::vector<MEdge> > &boundaries)
{

  //We suppose MElements are simply connected

  std::set<MEdge, Less_Edge> es;
  std::set<MVertex*> vs;
  int N = 0;
  for(unsigned int i = 0; i < elements.size(); i++){
    N++;
    MElement *e = elements[i];
    for(int j = 0; j < e->getNumVertices(); j++){
      vs.insert(e->getVertex(j));
    }
    for(int j = 0; j < e->getNumEdges(); j++){
      es.insert(e->getEdge(j));
    }
  }

  int poincare = vs.size() - es.size() + N;

  //compute connected boundaries
  int nbBounds = 0;
  std::vector<MEdge> bEdges;
  for(unsigned int i = 0; i < elements.size(); i++){
    for(int j = 0; j < elements[i]->getNumEdges(); j++){
      MEdge me =  elements[i]->getEdge(j);
      if(std::find(bEdges.begin(), bEdges.end(), me) == bEdges.end())
         bEdges.push_back(me);
      else
         bEdges.erase(std::find(bEdges.begin(), bEdges.end(),me));
    }
  }
  nbBounds = connected_bounds(bEdges, boundaries);
  int genus = (int)(-poincare + 2 - nbBounds)/2;

  //printf("************** partition has %d boundaries and genus =%d \n", nbBounds, genus);

  return genus;

}

static int getAspectRatio(std::vector<MElement *> &elements,
                          std::vector<std::vector<MEdge> > &boundaries)
{

  double area3D = 0.0;
  for(unsigned int i = 0; i <elements.size(); ++i){
    MElement *t = elements[i];
    std::vector<MVertex *> v(3);
    for(int k = 0; k < 3; k++) v[k] = t->getVertex(k);
    double p0[3] = {v[0]->x(), v[0]->y(), v[0]->z()};
    double p1[3] = {v[1]->x(), v[1]->y(), v[1]->z()};
    double p2[3] = {v[2]->x(), v[2]->y(), v[2]->z()};
    double a_3D = fabs(triangle_area(p0, p1, p2));
    area3D += a_3D;
  }

  double tot_length = 0.0;
  for(unsigned int i = 0; i <boundaries.size(); ++i){
    std::vector<MEdge> iBound = boundaries[i];
    double iLength = 0.0;
    for( unsigned int j = 0; j <iBound.size(); ++j){
      MVertex *v0 = iBound[j].getVertex(0);
      MVertex *v1 = iBound[j].getVertex(1);
      const double length = sqrt((v0->x() - v1->x()) * (v0->x() - v1->x()) +
                                 (v0->y() - v1->y()) * (v0->y() - v1->y()) +
                                 (v0->z() - v1->z()) * (v0->z() - v1->z()));
      iLength += length;
    }
    tot_length += iLength;
  }
  int AR = 1;
  if (boundaries.size() > 0){
    tot_length /= boundaries.size();
    AR = (int) ceil(2*3.14*area3D/(tot_length*tot_length));
  }

  //compute AR also with Bounding box
  std::set<MVertex*> vs;
  for(unsigned int i = 0; i < elements.size(); i++){
    MElement *e = elements[i];
    for(int j = 0; j < e->getNumVertices(); j++){
      vs.insert(e->getVertex(j));
    }
  }
  SBoundingBox3d bb;
  std::vector<SPoint3> vertices;
  for (std::set<MVertex* >::iterator it = vs.begin(); it != vs.end(); it++){
    SPoint3 pt((*it)->x(),(*it)->y(), (*it)->z());
    vertices.push_back(pt);
    bb += pt;
  }
  double H = norm(SVector3(bb.max(), bb.min()));

  //SOrientedBoundingBox obbox =  SOrientedBoundingBox::buildOBB(vertices);
  //double H = obbox.getMaxSize();

  double D = H;
  if (boundaries.size()  > 0 ) D = 10e4;
  for (unsigned int i = 0; i < boundaries.size(); i++){
    std::set<MVertex*> vb;
    std::vector<MEdge> iBound = boundaries[i];
    for (unsigned int j = 0; j < iBound.size(); j++){
      MEdge e = iBound[j];
      vb.insert(e.getVertex(0));
      vb.insert(e.getVertex(1));
    }
    std::vector<SPoint3> vBounds;
    SBoundingBox3d bb;
    for (std::set<MVertex* >::iterator it = vb.begin(); it != vb.end(); it++){
      SPoint3 pt((*it)->x(),(*it)->y(), (*it)->z());
      vBounds.push_back(pt);
      bb +=pt;
    }
    double iD = norm(SVector3(bb.max(), bb.min()));
    D = std::min(D, iD);

    //SOrientedBoundingBox obboxD = SOrientedBoundingBox::buildOBB(vBounds);
    //D = std::max(D, obboxD.getMaxSize());
  }
  int AR2 = (int)ceil(H/D);

  return std::max(AR, AR2);
}

static void getGenusAndRatio(std::vector<MElement *> &elements, int & genus, int &AR, int &NB)
{
  std::vector<std::vector<MEdge> > boundaries;
  boundaries.clear();
  genus = getGenus(elements, boundaries);
  NB = boundaries.size();
  AR = getAspectRatio(elements, boundaries);

}
static void partitionRegions(std::vector<MElement*> &elements,
                             std::vector<std::vector<MElement*> > &regions)
{

  for (unsigned int i = 0; i < elements.size(); ++i){
    MElement *e = elements[i];
    int part = e->getPartition();
    regions[part-1].push_back(e);
  }

  std::vector<std::vector<MElement*> > allRegions;
  for (unsigned int k = 0; k < regions.size(); ++k){
    std::vector<std::vector<MElement*> >  conRegions;
    conRegions.clear();
    connectedRegions (regions[k], conRegions);
    for (unsigned int j = 0; j < conRegions.size(); j++)
      allRegions.push_back(conRegions[j]);
  }
  regions.clear();
  regions.resize(allRegions.size());
  regions = allRegions;

}

/*
static void printLevel(std::vector<MElement *> &elements, int recur, int region)
{
  char fn[256];
  sprintf(fn, "part_%d_%d.msh", recur, region);
  double version = 2.2;

  std::set<MVertex*> vs;
  for (unsigned int i = 0; i < elements.size(); i++){
    for (int j = 0; j < elements[i]->getNumVertices(); j++){
      vs.insert(elements[i]->getVertex(j));
    }
  }

  bool binary = false;
  FILE *fp = Fopen (fn, "w");
  if(fp){
    fprintf(fp, "$MeshFormat\n");
    fprintf(fp, "%g %d %d\n", version, binary ? 1 : 0, (int)sizeof(double));
    fprintf(fp, "$EndMeshFormat\n");

    fprintf(fp,"$Nodes\n%d\n", (int)vs.size());
    std::set<MVertex*> :: iterator it = vs.begin();
    int index = 1;
    for (; it != vs.end() ; ++it){
      (*it)->setIndex(index++);
      fprintf(fp,"%d %g %g %g\n",(*it)->getIndex(),
              (*it)->x(),(*it)->y(),(*it)->z());
    }
    fprintf(fp,"$EndNodes\n");

    fprintf(fp,"$Elements\n%d\n", (int)elements.size());
    for (unsigned int i = 0; i < elements.size(); i++){
      elements[i]->writeMSH(fp, version);
    }
    fprintf(fp,"$EndElements\n%d\n", (int)elements.size());

    fclose(fp);
  }
}
*/

multiscalePartition::multiscalePartition(std::vector<MElement *> &elements,
                                         int nbParts, typeOfPartition method,
					 int allowPartition)
{
  options = CTX::instance()->partitionOptions;
  options.num_partitions = nbParts;
  options.partitioner = 1; //1 CHACO, 2 METIS
  if (options.partitioner == 1){
    options.global_method = 1;// 1 Multilevel-KL, 2 Spectral
    options.mesh_dims[0] = nbParts;
  }
  else if (options.partitioner == 2){
    options.algorithm = 2;//1 recursive, 2=kway, 3=nodal weights
    options.refine_algorithm=2;
    options.edge_matching = 3;
  }

  partitionLevel *level = new partitionLevel;
  level->elements.insert(level->elements.begin(),elements.begin(),elements.end());
  level->recur = 0;
  level->region = 0;

  levels.push_back(level);
  onlyMultilevel = false;
  if (allowPartition == 2)  onlyMultilevel = true;

  partition(*level, nbParts, method);

  totalParts = assembleAllPartitions(elements);

}

void multiscalePartition::setNumberOfPartitions(int &nbParts)
{
  options.num_partitions = nbParts;
   if (options.partitioner == 1){
     options.mesh_dims[0] = nbParts;
   }
}

void multiscalePartition::partition(partitionLevel & level, int nbParts,
                                    typeOfPartition method)
{
#if defined(HAVE_SOLVER) && defined(HAVE_ANN) && (defined(HAVE_METIS) || defined(HAVE_CHACO))

  if (method == LAPLACIAN){
    std::map<MVertex*, SPoint3> coordinates;
    multiscaleLaplace multiLaplace(level.elements, coordinates);
  }
  else if (method == MULTILEVEL){
    setNumberOfPartitions(nbParts);
    PartitionMeshElements(level.elements, options);
  }

  std::vector<std::vector<MElement*> > regions(nbParts);
  partitionRegions(level.elements, regions);
  level.elements.clear();

  for (unsigned i=0;i< regions.size() ; i++){

    partitionLevel *nextLevel = new partitionLevel;
    nextLevel->elements = regions[i];
    nextLevel->recur = level.recur+1;
    nextLevel->region = i;

    levels.push_back(nextLevel);
    int genus, AR, NB;
    getGenusAndRatio(regions[i], genus, AR, NB);

    if (genus < 0) {
      Msg::Error("Genus partition is negative G=%d!", genus);
      return;
    }

    if (genus != 0 ){
      int nbParts = std::max(genus+2,2);
      Msg::Info("Mesh partition: level (%d-%d)  is %d-GENUS (AR=%d) "
                "---> MULTILEVEL partition %d parts",
                nextLevel->recur,nextLevel->region, genus, AR, nbParts);
      partition(*nextLevel, nbParts, MULTILEVEL);
    }
    else if ((genus == 0  &&  AR > AR_MAX) || (genus == 0  &&  NB > 1)){
      int nbParts = 2;
      if(!onlyMultilevel){
	Msg::Info("Mesh partition: level (%d-%d)  is ZERO-GENUS (AR=%d NB=%d) "
                  "---> LAPLACIAN partition %d parts",
		  nextLevel->recur,nextLevel->region, AR, NB, nbParts);
	partition(*nextLevel, nbParts, LAPLACIAN);
      }
      else {
        Msg::Info("Mesh partition: level (%d-%d)  is ZERO-GENUS (AR=%d NB=%d) "
                  "---> MULTILEVEL partition %d parts",
                  nextLevel->recur,nextLevel->region, AR, NB, nbParts);
        partition(*nextLevel, nbParts, MULTILEVEL);
      }
    }
    else {
      Msg::Info("*** Mesh partition: level (%d-%d) is ZERO-GENUS (AR=%d, NB=%d)",
                nextLevel->recur,nextLevel->region, AR, NB);
    }

  }

#endif
}

int multiscalePartition::assembleAllPartitions(std::vector<MElement*> & elements)
{
  int iPart =  1;

  elements.clear();
  for (unsigned i = 0; i< levels.size(); i++){
    partitionLevel *iLevel = levels[i];
    if(iLevel->elements.size() > 0){
      for (unsigned j = 0; j < iLevel->elements.size(); j++){
        MElement *e = iLevel->elements[j];
	elements.push_back(e);
        e->setPartition(iPart);
      }
      iPart++;
    }
  }

  return iPart - 1;
}
