// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include "MPyramid.h"
#include "Numeric.h"
#include "Context.h"
#include "BergotBasis.h"
#include "BasisFactory.h"
#include "pointsGenerators.h"

std::map<int, indicesReversed> MPyramidN::_order2indicesReversedPyr;

int MPyramid::getVolumeSign()
{
  double mat[3][3];
  mat[0][0] = _v[1]->x() - _v[0]->x();
  mat[0][1] = _v[3]->x() - _v[0]->x();
  mat[0][2] = _v[4]->x() - _v[0]->x();
  mat[1][0] = _v[1]->y() - _v[0]->y();
  mat[1][1] = _v[3]->y() - _v[0]->y();
  mat[1][2] = _v[4]->y() - _v[0]->y();
  mat[2][0] = _v[1]->z() - _v[0]->z();
  mat[2][1] = _v[3]->z() - _v[0]->z();
  mat[2][2] = _v[4]->z() - _v[0]->z();
  double d = det3x3(mat);
  if(d < 0.) return -1;
  else if(d > 0.) return 1;
  else return 0;
}

void MPyramid::getIntegrationPoints(int pOrder, int *npts, IntPt **pts)
{
  *npts = getNGQPyrPts(pOrder);
  *pts = getGQPyrPts(pOrder);
}

MPyramidN::~MPyramidN() {}

int MPyramidN::getNumEdgesRep(bool curved)
{
  // FIXME: remove !getIsAssimilatedSerendipity() when serendip are implemented
  return (curved && !getIsAssimilatedSerendipity()) ?
    8 * CTX::instance()->mesh.numSubEdges : 8;
}

void MPyramidN::getEdgeRep(bool curved, int num,
                           double *x, double *y, double *z, SVector3 *n)
{
  // FIXME: remove !getIsAssimilatedSerendipity() when serendip are implemented
  if (curved && !getIsAssimilatedSerendipity()) {
    int numSubEdges = CTX::instance()->mesh.numSubEdges;
    static double pp[5][3] = {{-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},{0,0,1}};
    static int ed [8][2] = {{0,1},{0,3},{0,4},{1,2},{1,4},{2,3},{2,4},{3,4}};
    int iEdge = num / numSubEdges;
    int iSubEdge = num % numSubEdges;

    int iVertex1 = ed [iEdge][0];
    int iVertex2 = ed [iEdge][1];
    double t1 = (double) iSubEdge / (double) numSubEdges;
    double u1 = pp[iVertex1][0] * (1.-t1) + pp[iVertex2][0] * t1;
    double v1 = pp[iVertex1][1] * (1.-t1) + pp[iVertex2][1] * t1;
    double w1 = pp[iVertex1][2] * (1.-t1) + pp[iVertex2][2] * t1;

    double t2 = (double) (iSubEdge+1) / (double) numSubEdges;
    double u2 = pp[iVertex1][0] * (1.-t2) + pp[iVertex2][0] * t2;
    double v2 = pp[iVertex1][1] * (1.-t2) + pp[iVertex2][1] * t2;
    double w2 = pp[iVertex1][2] * (1.-t2) + pp[iVertex2][2] * t2;

    SPoint3 pnt1, pnt2;
    pnt(u1,v1,w1,pnt1);
    pnt(u2,v2,w2,pnt2);
    x[0] = pnt1.x(); x[1] = pnt2.x();
    y[0] = pnt1.y(); y[1] = pnt2.y();
    z[0] = pnt1.z(); z[1] = pnt2.z();

    // not great, but better than nothing
    static const int f[8] = {0, 1, 0, 2, 2, 3, 2, 3};
    n[0] = n[1] = getFace(f[iEdge]).normal();
  }
  else MPyramid::getEdgeRep(false, num, x, y, z, n);
}


int MPyramidN::getNumFacesRep(bool curved)
{
  // FIXME: remove !getIsAssimilatedSerendipity() when serendip are implemented
  return (curved && !getIsAssimilatedSerendipity()) ?
    6 * gmsh_SQU(CTX::instance()->mesh.numSubEdges) : 6;
}

static void _myGetFaceRep(MPyramid *pyr, int num, double *x, double *y, double *z,
                          SVector3 *n, int numSubEdges)
{
  static double pp[5][3] = {
    {-1,-1,0},
    {1,-1,0},
    {1,1,0},
    {-1,1,0},
    {0,0,1}
  };
  int iFace    = num / (numSubEdges * numSubEdges);
  int iSubFace = num % (numSubEdges * numSubEdges);

  if (iFace > 3) {
    iFace = 4;
    iSubFace = num % (2*numSubEdges * numSubEdges);
  }

  int iVertex1, iVertex2, iVertex3, iVertex4;

  if (iFace < 4) {
    iVertex1 = pyr->faces_pyramid(iFace,0);
    iVertex2 = pyr->faces_pyramid(iFace,1);
    iVertex3 = pyr->faces_pyramid(iFace,2);
  } else {
    iVertex1 = 0;
    iVertex2 = 1;
    iVertex3 = 2;
    iVertex4 = 3;
  }

  if (iFace < 4) {

    int ix = 0, iy = 0;
    int nbt = 0;
    for (int i = 0; i < numSubEdges; i++){
      int nbl = (numSubEdges - i - 1) * 2 + 1;
      nbt += nbl;
      if (nbt > iSubFace){
        iy = i;
        ix = nbl - (nbt - iSubFace);
        break;
      }
    }

    const double d = 1. / numSubEdges;

    SPoint3 pnt1, pnt2, pnt3;
    double u1, v1, u2, v2, u3, v3;
    if (ix % 2 == 0){
      u1 = ix / 2 * d; v1= iy*d;
      u2 = (ix / 2 + 1) * d ; v2 =  iy * d;
      u3 = ix / 2 * d ; v3 =  (iy+1) * d;
    }
    else{
      u1 = (ix / 2 + 1) * d; v1= iy * d;
      u2 = (ix / 2 + 1) * d; v2= (iy + 1) * d;
      u3 = ix / 2 * d ; v3 =  (iy + 1) * d;
    }

    double U1 = pp[iVertex1][0] * (1.-u1-v1) + pp[iVertex2][0] * u1 + pp[iVertex3][0] * v1;
    double U2 = pp[iVertex1][0] * (1.-u2-v2) + pp[iVertex2][0] * u2 + pp[iVertex3][0] * v2;
    double U3 = pp[iVertex1][0] * (1.-u3-v3) + pp[iVertex2][0] * u3 + pp[iVertex3][0] * v3;

    double V1 = pp[iVertex1][1] * (1.-u1-v1) + pp[iVertex2][1] * u1 + pp[iVertex3][1] * v1;
    double V2 = pp[iVertex1][1] * (1.-u2-v2) + pp[iVertex2][1] * u2 + pp[iVertex3][1] * v2;
    double V3 = pp[iVertex1][1] * (1.-u3-v3) + pp[iVertex2][1] * u3 + pp[iVertex3][1] * v3;

    double W1 = pp[iVertex1][2] * (1.-u1-v1) + pp[iVertex2][2] * u1 + pp[iVertex3][2] * v1;
    double W2 = pp[iVertex1][2] * (1.-u2-v2) + pp[iVertex2][2] * u2 + pp[iVertex3][2] * v2;
    double W3 = pp[iVertex1][2] * (1.-u3-v3) + pp[iVertex2][2] * u3 + pp[iVertex3][2] * v3;

    pyr->pnt(U1, V1, W1, pnt1);
    pyr->pnt(U2, V2, W2, pnt2);
    pyr->pnt(U3, V3, W3, pnt3);

    x[0] = pnt1.x(); x[1] = pnt2.x(); x[2] = pnt3.x();
    y[0] = pnt1.y(); y[1] = pnt2.y(); y[2] = pnt3.y();
    z[0] = pnt1.z(); z[1] = pnt2.z(); z[2] = pnt3.z();

    SVector3 d1(x[1] - x[0], y[1] - y[0], z[1] - z[0]);
    SVector3 d2(x[2] - x[0], y[2] - y[0], z[2] - z[0]);
    n[0] = crossprod(d1, d2);
    n[0].normalize();
    n[1] = n[0];
    n[2] = n[0];

  }
  else {
    SPoint3 pnt1, pnt2, pnt3;

    /*
    0
    0 1
    0 1 2
    0 1 2 3
    0 1 2 3 4
    0 1 2 3 4 5
    */

    // on each layer, we have (numSubEdges) * 2 triangles
    // ix and iy are the coordinates of the sub-quadrangle

    int io = iSubFace%2;
    int ix = (iSubFace/2)/numSubEdges;
    int iy = (iSubFace/2)%numSubEdges;

    const double d = 2. / numSubEdges;
    double ox = -1. + d*ix;
    double oy = -1. + d*iy;

    if (io == 0){
    double U1 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V1 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W1 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    ox += d;

    double U2 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V2 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W2 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    oy += d;

    double U3 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V3 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W3 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    pyr->pnt(U1,V1,W1, pnt1);
    pyr->pnt(U2,V2,W2, pnt2);
    pyr->pnt(U3,V3,W3, pnt3);
    }
    else{
    double U1 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V1 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W1 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    ox += d;
    oy += d;

    double U2 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V2 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W2 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    ox -= d;

    double U3 =
      pp[iVertex1][0] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][0] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][0] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][0] * (1.-ox)*(1+oy)*.25;
    double V3 =
      pp[iVertex1][1] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][1] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][1] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][1] * (1.-ox)*(1+oy)*.25;
    double W3 =
      pp[iVertex1][2] * (1.-ox)*(1-oy)*.25 +
      pp[iVertex2][2] * (1.+ox)*(1-oy)*.25 +
      pp[iVertex3][2] * (1.+ox)*(1+oy)*.25 +
      pp[iVertex4][2] * (1.-ox)*(1+oy)*.25;

    pyr->pnt(U1,V1,W1, pnt1);
    pyr->pnt(U2,V2,W2, pnt2);
    pyr->pnt(U3,V3,W3, pnt3);
    }

    n[0] = 1;
    n[1] = 1;
    n[2] = 1;
    x[0] = pnt1.x(); x[1] = pnt2.x(); x[2] = pnt3.x();
    y[0] = pnt1.y(); y[1] = pnt2.y(); y[2] = pnt3.y();
    z[0] = pnt1.z(); z[1] = pnt2.z(); z[2] = pnt3.z();
  }
}

void MPyramidN::getFaceRep(bool curved, int num,
                           double *x, double *y, double *z, SVector3 *n)
{
  // FIXME: remove !getIsAssimilatedSerendipity() when serendip are implemented
  if (curved && !getIsAssimilatedSerendipity())
    _myGetFaceRep(this, num, x, y, z, n, CTX::instance()->mesh.numSubEdges);
  else MPyramid::getFaceRep(false, num, x, y, z, n);
}

void _getIndicesReversedPyr(int order, indicesReversed &indices)
{
  fullMatrix<double> ref = gmshGenerateMonomialsPyramid(order);

  indices.resize(ref.size1());
  for (int i = 0; i < ref.size1(); ++i) {
    const double u = ref(i, 0);
    const double v = ref(i, 1);
    const double w = ref(i, 2);
    for (int j = 0; j < ref.size1(); ++j) {
      if (u == ref(j, 1) && v == ref(j, 0) && w == ref(j, 2)) {
        indices[i] = j;
        break;
      }
    }
  }
}

void MPyramidN::reverse()
{
  std::map<int, indicesReversed>::iterator it;
  it = _order2indicesReversedPyr.find(_order);
  if (it == _order2indicesReversedPyr.end()) {
    indicesReversed indices;
    _getIndicesReversedPyr(_order, indices);
    _order2indicesReversedPyr[_order] = indices;
    it = _order2indicesReversedPyr.find(_order);
  }

  indicesReversed &indices = it->second;

  // copy vertices
  std::vector<MVertex*> oldv(5 + _vs.size());
  std::copy(_v, _v+5, oldv.begin());
  std::copy(_vs.begin(), _vs.end(), oldv.begin()+5);

  // reverse
  for (int i = 0; i < 5; ++i) {
    _v[i] = oldv[indices[i]];
  }
  for (unsigned int i = 0; i < _vs.size(); ++i) {
    _vs[i] = oldv[indices[5+i]];
  }
}
