// Gmsh - Copyright (C) 1997-2014 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.
//
// Contributor(s):
//   Tristan Carrier Baudoin


#include "GmshConfig.h"
#include "surfaceFiller.h"
#include "Field.h"
#include "GModel.h"
#include "OS.h"
#include <queue>
#include <stack>

/// Here, we aim at producing a set of points that
/// enables to generate a nice quad mesh

#include "rtree.h"

#include "MVertex.h"
#include "MElement.h"
#include "BackgroundMesh.h"
#include "intersectCurveSurface.h"

#include "pointInsertionRTreeTools.h"

using namespace std;

// assume a point on the surface, compute the 4 possible neighbors.
//
//              ^ t2
//              |
//              |
//             v2
//              |
//              |
//       v1-----+------v3 -------> t1
//              |
//              |
//             v4
//
// we aim at generating a rectangle with sizes size_1 and size_2 along t1 and t2

bool compute4neighbors (GFace *gf,   // the surface
			MVertex *v_center, // the wertex for which we wnt to generate 4 neighbors
			SPoint2 &midpoint,
			bool goNonLinear, // do we compute the position in the real surface which is nonlinear
			SPoint2 newP[4][NUMDIR], // look into other directions
			SMetric3 &metricField, FILE *crossf = 0) // the mesh metric
{
  // we assume that v is on surface gf
  
  // get the parameter of the point on the surface
  reparamMeshVertexOnFace(v_center, gf, midpoint);
  
  double L = backgroundMesh::current()->operator()(midpoint[0],midpoint[1],0.0);
  //  printf("L = %12.5E\n",L);
  metricField = SMetric3(1./(L*L));
  FieldManager *fields = gf->model()->getFields();
  if(fields->getBackgroundField() > 0){
    Field *f = fields->get(fields->getBackgroundField());
     if (!f->isotropic()){
       (*f)(v_center->x(),v_center->y(),v_center->z(), metricField,gf);
     }
     else {
      L = (*f)(v_center->x(),v_center->y(),v_center->z(), gf);
      metricField = SMetric3(1./(L*L));
     }
  }


  // get the unit normal at that point
  Pair<SVector3, SVector3> der = gf->firstDer(SPoint2(midpoint[0],midpoint[1]));
  SVector3 s1 = der.first();
  SVector3 s2 = der.second();
  SVector3 n = crossprod(s1,s2);
  n.normalize();

  double M = dot(s1,s1);
  double N = dot(s2,s2);
  double E = dot(s1,s2);


  // compute the first fundamental form i.e. the metric tensor at the point
  // M_{ij} = s_i \cdot s_j
  double metric[2][2] = {{M,E},{E,N}};

  //  printf("%d %g %g %g\n",gf->tag(),s1.x(),s1.y(),s1.z());

  SVector3 basis_u = s1; basis_u.normalize();
  SVector3 basis_v = crossprod(n,basis_u);

  for (int DIR = 0 ; DIR < NUMDIR ; DIR ++){
    double quadAngle  = backgroundMesh::current()->getAngle (midpoint[0],midpoint[1],0) + DIRS[DIR];

    // normalize vector t1 that is tangent to gf at midpoint
    SVector3 t1 = basis_u * cos (quadAngle) + basis_v * sin (quadAngle) ;
    t1.normalize();

    // compute the second direction t2 and normalize (t1,t2,n) is the tangent frame
    SVector3 t2 = crossprod(n,t1);
    t2.normalize();
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),t1.x(),t1.y(),t1.z());
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),t2.x(),t2.y(),t2.z());
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),-t1.x(),-t1.y(),-t1.z());
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),-t2.x(),-t2.y(),-t2.z());

    double size_1 = sqrt(1. / dot(t1,metricField,t1));
    double size_2 = sqrt(1. / dot(t2,metricField,t2));

    // compute covariant coordinates of t1 and t2
    // t1 = a s1 + b s2 -->
    // t1 . s1 = a M + b E
    // t1 . s2 = a E + b N --> solve the 2 x 2 system
    // and get covariant coordinates a and b
    double rhs1[2] = {dot(t1,s1),dot(t1,s2)}, covar1[2];
    bool singular = false;
    if (!sys2x2(metric,rhs1,covar1)){
      Msg::Info("Argh surface %d %g %g %g -- %g %g %g -- %g %g",gf->tag(),s1.x(),s1.y(),s1.z(),s2.x(),s2.y(),s2.z(),size_1,size_2);
      covar1[1] = 1.0; covar1[0] = 0.0;
      singular = true;
    }
    double rhs2[2] = {dot(t2,s1),dot(t2,s2)}, covar2[2];
    if (!sys2x2(metric,rhs2,covar2)){
      Msg::Info("Argh surface %d %g %g %g -- %g %g %g",gf->tag(),s1.x(),s1.y(),s1.z(),s2.x(),s2.y(),s2.z());
      covar2[0] = 1.0; covar2[1] = 0.0;
      singular = true;
    }

    // transform the sizes with respect to the metric
    // consider a vector v of size 1 in the parameter plane
    // its length is sqrt (v^T M v) --> if I want a real size
    // of size1 in direction v, it should be sqrt(v^T M v) * size1
    double l1 = sqrt(covar1[0]*covar1[0]+covar1[1]*covar1[1]);
    double l2 = sqrt(covar2[0]*covar2[0]+covar2[1]*covar2[1]);

    covar1[0] /= l1;covar1[1] /= l1;
    covar2[0] /= l2;covar2[1] /= l2;

    double size_param_1  = size_1 / sqrt (  M*covar1[0]*covar1[0]+
						  2*E*covar1[1]*covar1[0]+
						  N*covar1[1]*covar1[1]);
    double size_param_2  = size_2 / sqrt (  M*covar2[0]*covar2[0]+
						  2*E*covar2[1]*covar2[0]+
						  N*covar2[1]*covar2[1]);
    if (singular){
      size_param_1 = size_param_2 = std::min (size_param_1,size_param_2);
    }

    const double EPS = 1.e-7;
    double r1 = EPS*(double)rand() / RAND_MAX;
    double r2 = EPS*(double)rand() / RAND_MAX;
    double r3 = EPS*(double)rand() / RAND_MAX;
    double r4 = EPS*(double)rand() / RAND_MAX;
    double r5 = EPS*(double)rand() / RAND_MAX;
    double r6 = EPS*(double)rand() / RAND_MAX;
    double r7 = EPS* (double)rand() / RAND_MAX;
    double r8 = EPS*(double)rand() / RAND_MAX;
    double newPoint[4][2] = {{midpoint[0] - covar1[0] * size_param_1 +r1,
			      midpoint[1] - covar1[1] * size_param_1 +r2},
			     {midpoint[0] - covar2[0] * size_param_2 +r3,
			      midpoint[1] - covar2[1] * size_param_2 +r4},
			     {midpoint[0] + covar1[0] * size_param_1 +r5,
			      midpoint[1] + covar1[1] * size_param_1 +r6},
			     {midpoint[0] + covar2[0] * size_param_2 +r7,
			      midpoint[1] + covar2[1] * size_param_2 +r8 }};
    // We could stop here. Yet, if the metric varies a lot, we can solve
    // a nonlinear problem in order to find a better approximation in the real
    // surface
    double ERR[4];
    for (int i=0;i<4;i++){
      GPoint pp = gf->point(SPoint2(newPoint[i][0], newPoint[i][1]));
      double D = sqrt ((pp.x() - v_center->x())*(pp.x() - v_center->x()) +
		       (pp.y() - v_center->y())*(pp.y() - v_center->y()) +
		       (pp.z() - v_center->z())*(pp.z() - v_center->z()) );
      ERR[i] = 100*fabs(D-L)/(D+L);
    }
    
    if (1 && goNonLinear){
      surfaceFunctorGFace ss (gf);                                        //
      SVector3 dirs[4] = {t1*(-1.0),t2*(-1.0),t1*(1.0),t2*(1.0)};                     //
      for (int i=0;i<4;i++){                                              //
	if (ERR[i] > 12){
	  double uvt[3] = {newPoint[i][0],newPoint[i][1],0.0};              //
	  //	  printf("Intersecting with circle N = %g %g %g dir = %g %g %g R = %g p = %g %g %g\n",n.x(),n.y(),n.z(),dirs[i].x(),dirs[i].y(),dirs[i].z(),L,v_center->x(),v_center->y(),v_center->z());
	  curveFunctorCircle cf (dirs[i],n,
				 SVector3(v_center->x(),v_center->y(),v_center->z()),
				 L);
	  if (intersectCurveSurface (cf,ss,uvt,size_param_1*1.e-3)){          //
	    GPoint pp = gf->point(SPoint2(uvt[0],uvt[1]));
	    double D = sqrt ((pp.x() - v_center->x())*(pp.x() - v_center->x()) +
			     (pp.y() - v_center->y())*(pp.y() - v_center->y()) +
			     (pp.z() - v_center->z())*(pp.z() - v_center->z()) );
	    double DP = sqrt ((newPoint[i][0]-uvt[0])*(newPoint[i][0]-uvt[0]) +
			      (newPoint[i][1]-uvt[1])*(newPoint[i][1]-uvt[1]));
	    double newErr = 100*fabs(D-L)/(D+L);
	    //	    if (v_center->onWhat() != gf && gf->tag() == 3){
	    //	      crossField2d::normalizeAngle (uvt[2]);
	    //	      printf("INTERSECT angle = %g DP %g\n",uvt[2],DP);
	    //	    }
	    if (newErr < 1 && DP < .1){
	      //	      printf("%12.5E vs %12.5E : %12.5E  %12.5E vs %12.5E  %12.5E \n",ERR[i],newErr,newPoint[i][0],newPoint[i][1],uvt[0],uvt[1]);
	      newPoint[i][0] = uvt[0];                                        //
	      newPoint[i][1] = uvt[1];                                        //
	    }                                                                 //
	    //	    printf("OK\n");
	  }
	  else{
	    Msg::Debug("Cannot put a new point on Surface %d",gf->tag());
	    //	    printf("NOT OK\n");
	  }
	}
      }                                                                   //
    } /// end non linear -------------------------------------------------//

    // return the four new vertices
    for (int i=0;i<4;i++){
      newP[i][DIR] = SPoint2(newPoint[i][0],newPoint[i][1]);
    }
  }
  return true;
}


// recover element around vertex v and interpolate smoothness on this element...
double get_smoothness(MVertex *v, GFace *gf, const map<MVertex*,double> &vertices2smoothness){
  // recover element around MVertex v
  //cout << "Looking for element around point (" << v->x() << "," << v->y() << "," << v->z() << ")" << endl;
  SPoint3 sp3(v->x(), v->y(), v->z());
  SPoint2 param_point;
  reparamMeshVertexOnFace(v, gf, param_point);
  MElement *elem = backgroundMesh::current()->getMeshElementByCoord(param_point[0], param_point[1], 0.);
  if (!elem){
    elem = backgroundMesh::current()->getMeshElementByCoord(param_point[0], param_point[1], 0., false);
    if (!elem)
      cout << " ------ WARNING !!! surfaceFiller : get_smoothness : No element found for coordinate (" << sp3.x() << "," << sp3.y() << "," << sp3.z() << ")" << endl;
  }

  // recover element's vertices:
  vector<MVertex*> localvertices;
  for (int ivert=0;ivert<elem->getNumVertices();ivert++){
    MVertex *temp = elem->getVertex(ivert);
    localvertices.push_back(temp);
//    cout << " made of vertex " << temp->x() << "," << temp->y() << "," << temp->z() << endl;
  }

  // recover parametrisation uvw
  double uvw[3],xyz[3];
  xyz[0] = param_point[0];
  xyz[1] = param_point[1];
  xyz[2] = 0.;
  elem->xyz2uvw(xyz, uvw);
//  cout << "xyz is " << xyz[0] << ","  << xyz[1] << ","  << xyz[2] << endl;
//  cout << "uvw is " << uvw[0] << ","  << uvw[1] << ","  << uvw[2] << endl;

  // interpolate :
  double val[3];
  int i=0;
  for (vector<MVertex*>::iterator it = localvertices.begin();it!=localvertices.end();it++){
    MVertex *localv = *it;
    map<MVertex*,double>::const_iterator itfind = vertices2smoothness.find(localv);
    if (itfind==vertices2smoothness.end()){
      cerr << "WARNING: surfaceFiller : get_smoothness : BACKGROUNDMESH VERTEX NOT FOUND IN SMOOTHNESS COMPUTATION !!! ABORTING..." << endl;
      throw;
    }
//    cout << "nodal value: " << itfind->second << endl;
    val[i++] = itfind->second;
  }
//  cout << "uvw is " << uvw[0] << "  " <<  uvw[1] << "  " <<  uvw[2] << endl;
  double res = elem->interpolate(val, uvw[0], uvw[1], uvw[2]);
//  cout << " THE VALUE = " << res << endl;
  return res;
}


void print_nodal_info_int(string filename, map<MVertex*, int> &mapp){
  ofstream out(filename.c_str());

  out << "View \"\"{" << endl;
  for (map<MVertex*, int>::iterator it = mapp.begin();it!=mapp.end();it++){
    MVertex *v = it->first;
    out << "SP( " << v->x() << "," << v->y() << "," << v->z() << "){" << it->second << "};" << endl;;
  }
  out << "};" << endl;

  out.close();
}


void print_nodal_info_double(string filename, map<MVertex*, double> &mapp){
  ofstream out(filename.c_str());

  out << "View \"\"{" << endl;
  for (map<MVertex*, double>::iterator it = mapp.begin();it!=mapp.end();it++){
    MVertex *v = it->first;
    out << "SP( " << v->x() << "," << v->y() << "," << v->z() << "){" << it->second << "};" << endl;;
  }
  out << "};" << endl;

  out.close();
}


void export_point(surfacePointWithExclusionRegion *sp, int DIR, FILE *crossf, GFace *gf){
  // get the unit normal at that point
  Pair<SVector3, SVector3> der = gf->firstDer(sp->_center);
  SVector3 s1 = der.first();
  SVector3 s2 = der.second();
  SVector3 n = crossprod(s1,s2);
  n.normalize();
  SVector3 basis_u = s1; basis_u.normalize();
  SVector3 basis_v = crossprod(n,basis_u);

  double quadAngle  = backgroundMesh::current()->getAngle (sp->_center[0],sp->_center[1],0) + DIRS[DIR];

  // normalize vector t1 that is tangent to gf at midpoint
  SVector3 t1 = basis_u * cos (quadAngle) + basis_v * sin (quadAngle) ;
  t1.normalize();

  // compute the second direction t2 and normalize (t1,t2,n) is the tangent frame
  SVector3 t2 = crossprod(n,t1);
  t2.normalize();

  //  double scale = DIR+1.;
  SMetric3 metricField;
  double L = backgroundMesh::current()->operator()(sp->_center[0],sp->_center[1],0.0);
  metricField = SMetric3(1./(L*L));
  FieldManager *fields = gf->model()->getFields();
  if(fields->getBackgroundField() > 0){
    Field *f = fields->get(fields->getBackgroundField());
    if (!f->isotropic()){
      (*f)(sp->_v->x(),sp->_v->y(),sp->_v->z(), metricField,gf);
    }
    else {
      L = (*f)(sp->_v->x(),sp->_v->y(),sp->_v->z(), gf);
      metricField = SMetric3(1./(L*L));
    }
  }
  double size_1 = sqrt(1. / dot(t1,metricField,t1));
  double size_2 = sqrt(1. / dot(t2,metricField,t2));

  //  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),t1.x()*scale,t1.y()*scale,t1.z()*scale);
  //  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),t2.x()*scale,t2.y()*scale,t2.z()*scale);
  //  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),-t1.x()*scale,-t1.y()*scale,-t1.z()*scale);
  //  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),-t2.x()*scale,-t2.y()*scale,-t2.z()*scale);
  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),t1.x()*size_1,t1.y()*size_1,t1.z()*size_1);
  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),t2.x()*size_2,t2.y()*size_2,t2.z()*size_2);
  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),-t1.x()*size_1,-t1.y()*size_1,-t1.z()*size_1);
  fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",sp->_v->x(),sp->_v->y(),sp->_v->z(),-t2.x()*size_2,-t2.y()*size_2,-t2.z()*size_2);
}


bool get_local_sizes_and_directions(const MVertex *v_center, const SPoint2 &midpoint, const int DIR, GFace* gf, double (&covar1)[2], double (&covar2)[2], double &size_param_1, double &size_param_2, double &L, SVector3 &t1, SVector3 &t2, SVector3 &n, FILE *crossf=NULL){
//bool get_RK_stuff(const MVertex *v_center, const SPoint2 &midpoint, const int DIR, GFace* gf, double (&covar1)[2], double (&covar2)[2], double &size_param_1, double &size_param_2, double &L, SVector3 &t1, SVector3 &t2, SVector3 &n, FILE *crossf, const SVector3 &previous_t1, const SVector3 &previous_t2, bool use_previous_basis=false, bool export_cross=true){

  // !!!!!!!!!!!! check if point is in domain (for RK !!!)
  if (!backgroundMesh::current()->inDomain(midpoint.x(),midpoint.y(),0)) return false;

  SMetric3 metricField;
  L = backgroundMesh::current()->operator()(midpoint[0],midpoint[1],0.0);
  //  printf("L = %12.5E\n",L);
  metricField = SMetric3(1./(L*L));
  FieldManager *fields = gf->model()->getFields();
  if(fields->getBackgroundField() > 0){
    Field *f = fields->get(fields->getBackgroundField());
    if (!f->isotropic()){
      (*f)(v_center->x(),v_center->y(),v_center->z(), metricField,gf);
    }
    else {
      L = (*f)(v_center->x(),v_center->y(),v_center->z(), gf);
      metricField = SMetric3(1./(L*L));
    }
  }

  // get the unit normal at that point
  Pair<SVector3, SVector3> der = gf->firstDer(SPoint2(midpoint[0],midpoint[1]));
  SVector3 s1 = der.first();
  SVector3 s2 = der.second();
  n = crossprod(s1,s2);
  n.normalize();

  double M = dot(s1,s1);
  double N = dot(s2,s2);
  double E = dot(s1,s2);

  // compute the first fundamental form i.e. the metric tensor at the point
  // M_{ij} = s_i \cdot s_j
  double metric[2][2] = {{M,E},{E,N}};

  //  printf("%d %g %g %g\n",gf->tag(),s1.x(),s1.y(),s1.z());

  SVector3 basis_u = s1; basis_u.normalize();
  SVector3 basis_v = crossprod(n,basis_u);

  double quadAngle  = backgroundMesh::current()->getAngle (midpoint[0],midpoint[1],0) + DIRS[DIR];
  //double quadAngle  = atan2(midpoint[0],midpoint[1]);

  // normalize vector t1 that is tangent to gf at midpoint
  t1 = basis_u * cos (quadAngle) + basis_v * sin (quadAngle) ;
  t1.normalize();

  // compute the second direction t2 and normalize (t1,t2,n) is the tangent frame
  t2 = crossprod(n,t1);
  t2.normalize();


  //  std::cout << std::endl;
  //  std::cout << "basis uv : (" << basis_u(0) << "," <<  basis_u(1) << ") (" << basis_v(0) << "," << basis_v(1) << std::endl;
  //  std::cout << "t        : (" << t1(0) << "," <<  t1(1) << ") (" << t2(0) << "," << t2(1) << std::endl;

//  if (use_previous_basis){
//    std::map<double, double> angles;
//    SVector3 temp = crossprod(previous_t1, t1);
//    double a = atan2(dot(t1, previous_t1), sign(dot(temp,n))*temp.norm() );
//    angles.insert(std::make_pair(abs(a),a));
//    temp = crossprod(previous_t2, t1);
//    a = atan2(dot(t1, previous_t2), sign(dot(temp,n))*temp.norm());
//    angles.insert(std::make_pair(abs(a),a));
//    temp = crossprod(-1.*previous_t1, t1);
//    a = atan2(dot(t1, -1.*previous_t1), sign(dot(temp,n))*temp.norm());
//    angles.insert(std::make_pair(abs(a),a));
//    temp = crossprod(-1.*previous_t2, t1);
//    a = atan2(dot(t1, -1.*previous_t2), sign(dot(temp,n))*temp.norm());
//    angles.insert(std::make_pair(abs(a),a));
//    //    std::cout << "angles: " << std::endl;
//    //    for (int i=0;i<4;i++)  std::cout << angles[i] << "  " << std::endl;
//    double min_angle = -(angles.begin()->second);
//    //    std::cout << "min angle = " << min_angle << std::endl;
//    t1 =  cos(min_angle)*previous_t1 + sin(min_angle)*previous_t2;
//    t2 = -sin(min_angle)*previous_t1 + cos(min_angle)*previous_t2;
//    //    std::cout << "new corrected t        : (" << t1(0) << "," <<  t1(1) << ") (" << t2(0) << "," << t2(1) << std::endl;
//  }

  double size_1 = sqrt(1. / dot(t1,metricField,t1));
  double size_2 = sqrt(1. / dot(t2,metricField,t2));


  if (crossf){
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),t1.x()*size_1,t1.y()*size_1,t1.z()*size_1);
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),t2.x()*size_2,t2.y()*size_2,t2.z()*size_2);
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),-t1.x()*size_1,-t1.y()*size_1,-t1.z()*size_1);
    if (DIR == 0 && crossf)fprintf(crossf,"VP(%g,%g,%g) {%g,%g,%g};\n",v_center->x(),v_center->y(),v_center->z(),-t2.x()*size_2,-t2.y()*size_2,-t2.z()*size_2);
  }

  // compute covariant coordinates of t1 and t2
  // t1 = a s1 + b s2 -->
  // t1 . s1 = a M + b E
  // t1 . s2 = a E + b N --> solve the 2 x 2 system
  // and get covariant coordinates a and b
  double rhs1[2] = {dot(t1,s1),dot(t1,s2)};
  bool singular = false;
  if (!sys2x2(metric,rhs1,covar1)){
    Msg::Info("Argh surface %d %g %g %g -- %g %g %g -- %g %g",gf->tag(),s1.x(),s1.y(),s1.z(),s2.x(),s2.y(),s2.z(),size_1,size_2);
    covar1[1] = 1.0; covar1[0] = 0.0;
    singular = true;
  }
  double rhs2[2] = {dot(t2,s1),dot(t2,s2)};
  if (!sys2x2(metric,rhs2,covar2)){
    Msg::Info("Argh surface %d %g %g %g -- %g %g %g",gf->tag(),s1.x(),s1.y(),s1.z(),s2.x(),s2.y(),s2.z());
    covar2[0] = 1.0; covar2[1] = 0.0;
    singular = true;
  }

  // transform the sizes with respect to the metric
  // consider a vector v of size 1 in the parameter plane
  // its length is sqrt (v^T M v) --> if I want a real size
  // of size1 in direction v, it should be sqrt(v^T M v) * size1
  double l1 = sqrt(covar1[0]*covar1[0]+covar1[1]*covar1[1]);
  double l2 = sqrt(covar2[0]*covar2[0]+covar2[1]*covar2[1]);

  covar1[0] /= l1;covar1[1] /= l1;
  covar2[0] /= l2;covar2[1] /= l2;

  size_param_1  = size_1 / sqrt (  M*covar1[0]*covar1[0]+
      2*E*covar1[1]*covar1[0]+
      N*covar1[1]*covar1[1]);
  size_param_2  = size_2 / sqrt (  M*covar2[0]*covar2[0]+
      2*E*covar2[1]*covar2[0]+
      N*covar2[1]*covar2[1]);
  if (singular){
    size_param_1 = size_param_2 = std::min (size_param_1,size_param_2);
  }


  return true;
}



// using fifo based on smoothness criteria
void packingOfParallelogramsSmoothness(GFace* gf,  std::vector<MVertex*> &packed, std::vector<SMetric3> &metrics){
  cout << endl << "------------------------------------------" << endl << "   PACKINGOFPARALLELOGRAMS: NEW ALGO BASED ON SMOOTHNESS" << endl << "------------------------------------------" << endl;
  const bool goNonLinear = true;

  const bool debug = false;

  // build vertex -> neighbors table
  multimap<MVertex*,MVertex*> vertex2vertex;
  for (std::vector<MElement*>::iterator it = backgroundMesh::current()->begin_triangles();it!=backgroundMesh::current()->end_triangles();it++){
    MElement *e = *it;
    for (int i=0;i<e->getNumVertices();i++){
      MVertex *current = e->getVertex(i);
      for (int j=0;j<e->getNumVertices();j++){
        if (i==j) continue;
        MVertex *neighbor = e->getVertex(j);
        vertex2vertex.insert(make_pair(current,neighbor));
      }
    }
  }

  // build table vertex->smoothness
  map<MVertex*,double> vertices2smoothness;
  map<MVertex*, double> smoothness_essai;
  for (std::vector<MVertex*>::iterator it = backgroundMesh::current()->begin_vertices();it!=backgroundMesh::current()->end_vertices();it++){
    MVertex *v = *it;

    SPoint2 param_point(v->x(),v->y());GPoint gpt = gf->point(param_point); MVertex v_real(gpt.x(),gpt.y(),gpt.z());
    SVector3 t1,t2,n;double covar1[2],covar2[2],L,size_param_1,size_param_2;
    get_local_sizes_and_directions(&v_real, param_point, 0, gf, covar1, covar2, size_param_1, size_param_2, L, t1, t2, n);

    // compare to all neighbors...
    pair<multimap<MVertex*,MVertex*>::iterator, multimap<MVertex*,MVertex*>::iterator> range = vertex2vertex.equal_range(v);
    SVector3 t1_nb,t2_nb,n_nb;double covar1_nb[2],covar2_nb[2],L_nb,size_param_1_nb,size_param_2_nb;
    double maxprod,angle=0.;
    int N=0;
    for (multimap<MVertex*,MVertex*>::iterator itneighbor = range.first;itneighbor!=range.second;itneighbor++){
      N++;
      maxprod=0.;
      MVertex *v_nb = itneighbor->second;
      SPoint2 param_point_nb(v_nb->x(),v_nb->y());GPoint gpt_nb = gf->point(param_point_nb); MVertex v_real_nb(gpt_nb.x(),gpt_nb.y(),gpt_nb.z());
      get_local_sizes_and_directions(&v_real_nb, param_point_nb, 0, gf, covar1_nb, covar2_nb, size_param_1_nb, size_param_2_nb, L_nb, t1_nb, t2_nb, n_nb);
      // angle comparison...
      maxprod = std::max(maxprod, fabs(t1[0]*t1_nb[0] + t1[1]*t1_nb[1]));
      maxprod = std::max(maxprod, fabs(t1[0]*t2_nb[0] + t1[1]*t2_nb[1]));
      angle += fabs(acos(max(min(maxprod,1.),-1.)));
    }
    angle /= N;
    vertices2smoothness[v] = angle;
  }

//  if (debug){
//    stringstream ss;
//    ss << "backgroundmesh_smoothness_" << gf->tag() << ".pos";
//    backgroundMesh::current()->print(ss.str().c_str(),gf, vertices2smoothness);
//  }


  // --------------- export de smoothness comme elements.... -----------------------
  if (debug){
    stringstream ss;
    ss << "backgroundmesh_element_smoothness_" << gf->tag() << ".pos";
    ofstream out(ss.str().c_str());
    out << "View \"directions\" {" << endl;
    for (std::vector<MElement*>::iterator it = backgroundMesh::current()->begin_triangles();it!=backgroundMesh::current()->end_triangles();it++){
      MElement *e = *it;
      vector<MVertex *> nodes;
      vector<double> smoothtemp;
      for (int i=0;i<3;i++){
        MVertex *v = e->getVertex(i);
        nodes.push_back(v);
        smoothtemp.push_back(vertices2smoothness[v]);
      }
      out << "ST(";
      for (int i=0;i<3;i++){
        GPoint pp = gf->point(SPoint2(nodes[i]->x(),nodes[i]->y()));
        out << pp.x() << "," << pp.y() << "," << pp.z();
        if (i!=2) out << ",";
      }
      out << "){";
      for (int i=0;i<3;i++){
        out << (1.-(smoothtemp[i]/M_PI*4.));
        if (i!=2) out << ",";
      }
      out << "};" << endl;

    }
    out << "};" << endl;
    out.close();
  }
  //                   --------------- END ----------------


  // for debug check...
  int priority_counter=0;
  map<MVertex*,int> vert_priority;

  // get all the boundary vertices
  std::set<MVertex*> bnd_vertices;
  for(unsigned int i=0;i<gf->getNumMeshElements();i++){
    MElement* element = gf->getMeshElement(i);
    for(int j=0;j<element->getNumVertices();j++){
      MVertex *vertex = element->getVertex(j);
      if (vertex->onWhat()->dim() < 2)bnd_vertices.insert(vertex);
    }
  }

  //  --------- put boundary vertices in a fifo queue ---------------
  std::set<smoothness_point_pair, compareSurfacePointWithExclusionRegionPtr_Smoothness> fifo;
  std::vector<surfacePointWithExclusionRegion*> vertices;
  // put the RTREE
  RTree<surfacePointWithExclusionRegion*,double,2,double> rtree;
  SMetric3 metricField(1.0);
  SPoint2 newp[4][NUMDIR];
  std::set<MVertex*>::iterator it =  bnd_vertices.begin() ;

  char NAME[345]; sprintf(NAME,"crossReal%d.pos",gf->tag());
  FILE *crossf = NULL;
  if (debug){
    crossf = Fopen (NAME,"w");
  }
  if (crossf) fprintf(crossf,"View \"\"{\n");
  for (; it !=  bnd_vertices.end() ; ++it){
    SPoint2 midpoint;
    //compute4neighbors_RK2 (gf, *it, midpoint, goNonLinear, newp, metricField,crossf);
    compute4neighbors(gf, *it, midpoint, goNonLinear, newp, metricField,crossf);
    surfacePointWithExclusionRegion *sp =
      new surfacePointWithExclusionRegion (*it, newp, midpoint,metricField);
    smoothness_point_pair mp;mp.ptr = sp;mp.rank=get_smoothness(*it,gf,vertices2smoothness);
    fifo.insert(mp);

    if (debug){
      smoothness_essai[*it] = mp.rank;
    }

    vertices.push_back(sp);
    double _min[2],_max[2];
    sp->minmax(_min,_max);
    rtree.Insert(_min,_max,sp);
    if (crossf) export_point(sp, 0, crossf, gf);
  }

  // ---------- main loop -----------------
  while(!fifo.empty()){
    if (debug) std::cout << " -------- fifo.size() = " << fifo.size() << std::endl;

    surfacePointWithExclusionRegion * parent = (*fifo.begin()).ptr;
    fifo.erase(fifo.begin());
    int count_nbaddedpt = 0;
    for (int dir=0;dir<NUMDIR;dir++){
      for (int i=0;i<4;i++){

        if (!inExclusionZone (parent->_p[i][dir], rtree, vertices) ){
          GPoint gp = gf->point(parent->_p[i][dir]);
          MFaceVertex *v = new MFaceVertex(gp.x(),gp.y(),gp.z(),gf,gp.u(),gp.v());
          SPoint2 midpoint;
          //compute4neighbors_RK2 (gf, v, midpoint, goNonLinear, newp, metricField,crossf);
          compute4neighbors(gf, v, midpoint, goNonLinear, newp, metricField,crossf);
          surfacePointWithExclusionRegion *sp =
            new surfacePointWithExclusionRegion (v, newp, midpoint, metricField, parent);
          smoothness_point_pair mp;mp.ptr = sp;mp.rank=get_smoothness(v,gf,vertices2smoothness);

          if (debug){
            smoothness_essai[v] = mp.rank;
            vert_priority[v] = priority_counter++;
          }
          fifo.insert(mp);
          vertices.push_back(sp);
          double _min[2],_max[2];
          sp->minmax(_min,_max);
          rtree.Insert(_min,_max,sp);
          if (crossf) export_point(sp, dir, crossf, gf);

          if (debug){
            std::cout << "  adding node (" << sp->_v->x() << "," << sp->_v->y() << "," << sp->_v->z() << ")" << std::endl;
            std::cout << "    ----------------------------- sub --- fifo.size() = " << fifo.size() << std::endl;
          }
          count_nbaddedpt++;
        }
      }
    }
    if (debug) std::cout << "////////// nbre of added point: " << count_nbaddedpt << std::endl;
  }
  if (crossf){
    fprintf(crossf,"};\n");
    fclose (crossf);
  }


  if (debug){
    stringstream ss;
    ss << "priority_" << gf->tag() << ".pos";
    print_nodal_info_int(ss.str().c_str(),vert_priority);
    ss.clear();
    ss << "smoothness_test_" << gf->tag() << ".pos";
    print_nodal_info_double(ss.str().c_str(),smoothness_essai);
  }


  // add the vertices as additional vertices in the
  // surface mesh
  //  char ccc[256]; sprintf(ccc,"points%d.pos",gf->tag());
  //  FILE *f = Fopen(ccc,"w");
  //  if(f) fprintf(f, "View \"\"{\n");
  for (unsigned int i=0;i<vertices.size();i++){
    //    if(f) vertices[i]->print(f,i);
    if(vertices[i]->_v->onWhat() == gf) {
      packed.push_back(vertices[i]->_v);
      metrics.push_back(vertices[i]->_meshMetric);
      SPoint2 midpoint;
      reparamMeshVertexOnFace(vertices[i]->_v, gf, midpoint);
    }
    delete  vertices[i];
  }
  //  if(f){
  //    fprintf(f,"};");
  //    fclose(f);
  //  }
}




// fills a surface with points in order to build a nice
// quad mesh ------------
void packingOfParallelograms(GFace* gf,  std::vector<MVertex*> &packed, std::vector<SMetric3> &metrics){
  //PE MODIF
//  packingOfParallelogramsSmoothness(gf,packed,metrics);
//  return;
  // END PE MODIF

  const bool goNonLinear = true;

  //  FILE *f = Fopen ("parallelograms.pos","w");


  // test test test
  stringstream ssa;
  ssa << "oldbgm_angles_" << gf->tag() << ".pos";
  backgroundMesh::current()->print(ssa.str(),gf,1);
  // test test test

  // get all the boundary vertices
  std::set<MVertex*> bnd_vertices;
  for(unsigned int i=0;i<gf->getNumMeshElements();i++){
    MElement* element = gf->getMeshElement(i);
    for(int j=0;j<element->getNumVertices();j++){
      MVertex *vertex = element->getVertex(j);
      if (vertex->onWhat()->dim() < 2)bnd_vertices.insert(vertex);
    }
  }

  // put boundary vertices in a fifo queue
  // std::queue<surfacePointWithExclusionRegion*> fifo;
  std::set<surfacePointWithExclusionRegion*,  compareSurfacePointWithExclusionRegionPtr> fifo;
  std::vector<surfacePointWithExclusionRegion*> vertices;
  // put the RTREE
  RTree<surfacePointWithExclusionRegion*,double,2,double> rtree;
  SMetric3 metricField(1.0);
  SPoint2 newp[4][NUMDIR];
  std::set<MVertex*>::iterator it =  bnd_vertices.begin() ;

  char NAME[345]; sprintf(NAME,"crossReal%d.pos",gf->tag());
  FILE *crossf = Fopen (NAME,"w");
  if (crossf) fprintf(crossf,"View \"\"{\n");
  for (; it !=  bnd_vertices.end() ; ++it){
    SPoint2 midpoint;
    compute4neighbors (gf, *it, midpoint, goNonLinear, newp, metricField,crossf);
    surfacePointWithExclusionRegion *sp =
      new surfacePointWithExclusionRegion (*it, newp, midpoint,metricField);
    //    fifo.push(sp);
    fifo.insert(sp);
    vertices.push_back(sp);
    double _min[2],_max[2];
    sp->minmax(_min,_max);
    //    printf("%g %g .. %g %g\n",_min[0],_min[1],_max[0],_max[1]);
    rtree.Insert(_min,_max,sp);
    //    sp->print(f);
  }

  //  printf("initially : %d vertices in the domain\n",vertices.size());

  
  while(!fifo.empty()){
    surfacePointWithExclusionRegion * parent = *fifo.begin();
    fifo.erase(fifo.begin());
    for (int dir=0;dir<NUMDIR;dir++){
      int countOK = 0;
      for (int i=0;i<4;i++){
        if (!inExclusionZone (parent->_p[i][dir], rtree, vertices) ){
          countOK++;
          GPoint gp = gf->point(parent->_p[i][dir]);
          MFaceVertex *v = new MFaceVertex(gp.x(),gp.y(),gp.z(),gf,gp.u(),gp.v());
          SPoint2 midpoint;
          compute4neighbors (gf, v, midpoint, goNonLinear, newp, metricField,crossf);
          surfacePointWithExclusionRegion *sp =
            new surfacePointWithExclusionRegion (v, newp, midpoint, metricField, parent);
          fifo.insert(sp);
          vertices.push_back(sp);
          double _min[2],_max[2];
          sp->minmax(_min,_max);
          rtree.Insert(_min,_max,sp);
        }
      }
      if (countOK)break;
      }
  }
  if (crossf){
    fprintf(crossf,"};\n");
    fclose (crossf);
  }
    // add the vertices as additional vertices in the
    // surface mesh
  char ccc[256]; sprintf(ccc,"points%d.pos",gf->tag());
  FILE *f = Fopen(ccc,"w");
  if(f) fprintf(f,"View \"\"{\n");
  for (unsigned int i=0;i<vertices.size();i++){
    //    if(vertices[i]->_v->onWhat() != gf)
    if(f) vertices[i]->print(f,i);
    if(vertices[i]->_v->onWhat() == gf) {
      packed.push_back(vertices[i]->_v);
      metrics.push_back(vertices[i]->_meshMetric);
      SPoint2 midpoint;
      reparamMeshVertexOnFace(vertices[i]->_v, gf, midpoint);
    }
    delete  vertices[i];
  }
  if(f){
    fprintf(f,"};");
    fclose(f);
  }
}


// fills a surface with points in order to build a nice
// quad mesh ------------
void packingOfParallelogramsConstrained(GFace* gf, std::set<MVertex*> constr_vertices, std::vector<MVertex*> &packed, std::vector<SMetric3> &metrics){
  //PE MODIF
//  packingOfParallelogramsSmoothness(gf,packed,metrics);
//  return;
  // END PE MODIF
#if defined(HAVE_RTREE)

	std::cout<<"      inside packingOfParallelogramsConstrained"<<std::endl;
  const bool goNonLinear = true;

  //  FILE *f = Fopen ("parallelograms.pos","w");

  // get all the boundary vertices
  std::set<MVertex*> bnd_vertices;
  for(unsigned int i=0;i<gf->getNumMeshElements();i++){
    MElement* element = gf->getMeshElement(i);
    for(int j=0;j<element->getNumVertices();j++){
      MVertex *vertex = element->getVertex(j);
      if (vertex->onWhat()->dim() < 2)bnd_vertices.insert(vertex);
    }
  }
  std::cout<<"      got all the boundary vertices"<<std::endl;

  // put boundary vertices in a fifo queue
  // std::queue<surfacePointWithExclusionRegion*> fifo;
  std::set<surfacePointWithExclusionRegion*,  compareSurfacePointWithExclusionRegionPtr> fifo;
  std::vector<surfacePointWithExclusionRegion*> vertices;
  // put the RTREE
  RTree<surfacePointWithExclusionRegion*,double,2,double> rtree;
  SMetric3 metricField(1.0);
  SPoint2 newp[4][NUMDIR];
  std::set<MVertex*>::iterator it =  bnd_vertices.begin() ;

  char NAME[345]; sprintf(NAME,"crossReal%d.pos",gf->tag());
  FILE *crossf = Fopen (NAME,"w");
  if (crossf)fprintf(crossf,"View \"\"{\n");
  std::cout<<"      entering first for"<<std::endl;
  for (; it !=  bnd_vertices.end() ; ++it){
    SPoint2 midpoint;
    compute4neighbors (gf, *it, midpoint, goNonLinear, newp, metricField,crossf);
    surfacePointWithExclusionRegion *sp =
      new surfacePointWithExclusionRegion (*it, newp, midpoint,metricField);
    //    fifo.push(sp);
    fifo.insert(sp);
    vertices.push_back(sp);
    double _min[2],_max[2];
    sp->minmax(_min,_max);
    //    printf("%g %g .. %g %g\n",_min[0],_min[1],_max[0],_max[1]);
    rtree.Insert(_min,_max,sp);
    //    sp->print(f);
  }

  std::set<MVertex*>::iterator it_constr =  constr_vertices.begin() ;
  std::cout<<"      entering second for"<<std::endl;
  for (; it_constr !=  constr_vertices.end() ; ++it_constr){
    SPoint2 midpoint;
    std::cout<<"going to test out parameterisation of the new point"<<std::endl;
    double para0, para1;
    (*it_constr)->getParameter(0,para0);
    (*it_constr)->getParameter(1,para1);
    std::cout<<"            point tested: para 1 "<<para0<<" and para 2 "<<para1<<std::endl;
    std::cout<<"         going to compute4neighbors"<<std::endl;
    compute4neighbors (gf, *it_constr, midpoint, goNonLinear, newp, metricField,crossf);
    std::cout<<"         going to surfacePointWithExclusionRegion"<<std::endl;
    surfacePointWithExclusionRegion *sp =
      new surfacePointWithExclusionRegion (*it_constr, newp, midpoint,metricField);
    //    fifo.push(sp);
    std::cout<<"         done surfacePointWithExclusionRegion"<<std::endl;
    fifo.insert(sp);
    vertices.push_back(sp);
    double _min[2],_max[2];
    sp->minmax(_min,_max);
    //    printf("%g %g .. %g %g\n",_min[0],_min[1],_max[0],_max[1]);
    rtree.Insert(_min,_max,sp);
    //    sp->print(f);
  }

  //  printf("initially : %d vertices in the domain\n",vertices.size());


  std::cout<<"      entering while"<<std::endl;
  while(!fifo.empty()){
    //surfacePointWithExclusionRegion & parent = fifo.top();
    //    surfacePointWithExclusionRegion * parent = fifo.front();
    surfacePointWithExclusionRegion * parent = *fifo.begin();
    //    fifo.pop();
    fifo.erase(fifo.begin());
    for (int dir=0;dir<NUMDIR;dir++){
      //      printf("dir = %d\n",dir);
      int countOK = 0;
      for (int i=0;i<4;i++){
        //	printf("i = %d %12.5E %12.5E \n",i,parent._p[i][dir].x(),parent._p[i][dir].y());

        //	if (!w._tooclose){
        if (!inExclusionZone (parent->_p[i][dir], rtree, vertices) ){
          countOK++;
          GPoint gp = gf->point(parent->_p[i][dir]);
          MFaceVertex *v = new MFaceVertex(gp.x(),gp.y(),gp.z(),gf,gp.u(),gp.v());
          std::cout<<"going to test out parameterisation of the new point"<<std::endl;
          double para0, para1;
          v->getParameter(0,para0);
          v->getParameter(1,para1);
//          if (v->getNum() == 341){
//        	  std::cout<<" 341 dans surface filler !!!!"<<std::endl;
//      		std::system("pause");
//      		getchar();
//          }
          std::cout<<"            point tested: para 1 "<<para0<<" and para 2 "<<para1<<std::endl;
          //	  	printf(" %g %g %g %g\n",parent._center.x(),parent._center.y(),gp.u(),gp.v());
          SPoint2 midpoint;
          compute4neighbors (gf, v, midpoint, goNonLinear, newp, metricField,crossf);
          surfacePointWithExclusionRegion *sp =
            new surfacePointWithExclusionRegion (v, newp, midpoint, metricField, parent);
          //	  fifo.push(sp);
          fifo.insert(sp);
          vertices.push_back(sp);
          double _min[2],_max[2];
          sp->minmax(_min,_max);
          rtree.Insert(_min,_max,sp);
        }
      }
      if (countOK)break;
      }
      //    printf("%d\n",vertices.size());
    }
  std::cout<<"      entering if"<<std::endl;
    if (crossf){
      fprintf(crossf,"};\n");
      fclose (crossf);
    }
    //  printf("done\n");

    // add the vertices as additional vertices in the
    // surface mesh
    char ccc[256]; sprintf(ccc,"points%d.pos",gf->tag());
    FILE *f = Fopen(ccc,"w");
    fprintf(f,"View \"\"{\n");
    std::cout<<"      entering another for"<<std::endl;
    for (unsigned int i=0;i<vertices.size();i++){
      //    if(vertices[i]->_v->onWhat() != gf)
      vertices[i]->print(f,i);
      if(vertices[i]->_v->onWhat() == gf) {
        packed.push_back(vertices[i]->_v);
        metrics.push_back(vertices[i]->_meshMetric);
        SPoint2 midpoint;
        reparamMeshVertexOnFace(vertices[i]->_v, gf, midpoint);
        std::cout<<"going to test out parameterisation of the REPARAM point"<<std::endl;
        double para0, para1;
        vertices[i]->_v->getParameter(0,para0);
        vertices[i]->_v->getParameter(1,para1);
        std::cout<<"            point tested: para 1 "<<para0<<" and para 2 "<<para1<<std::endl;
        //      fprintf(f,"TP(%22.15E,%22.15E,%g){%22.15E,%22.15E,%22.15E,%22.15E,%22.15E,%22.15E,%22.15E,%22.15E,%22.15E};\n",vertices[i]->_v->x(),vertices[i]->_v->y(),vertices[i]->_v->z(),
        //	      vertices[i]->_meshMetric(0,0),vertices[i]->_meshMetric(0,1),vertices[i]->_meshMetric(0,2),
        //	      vertices[i]->_meshMetric(1,0),vertices[i]->_meshMetric(1,1),vertices[i]->_meshMetric(1,2),
        //	      vertices[i]->_meshMetric(2,0),vertices[i]->_meshMetric(2,1),vertices[i]->_meshMetric(2,2));
        //fprintf(f,"SP(%22.15E,%22.15E,%g){1};\n",midpoint.x(),midpoint.y(),0.0);
      }
      delete  vertices[i];
    }
    fprintf(f,"};");
    fclose(f);
    //  printf("packed.size = %d\n",packed.size());
    //  delete rtree;
#endif
}
