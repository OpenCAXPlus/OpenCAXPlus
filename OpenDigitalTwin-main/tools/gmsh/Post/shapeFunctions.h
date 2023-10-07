// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _SHAPE_FUNCTIONS_H_
#define _SHAPE_FUNCTIONS_H_

#include "Numeric.h"
#include "GmshMessage.h"


class element{
protected:
  bool _ownData;
  double *_x, *_y, *_z;
  static double TOL;
public:
  element(double *x, double *y, double *z, int numNodes=0)
  {
    if(!numNodes){
      _ownData = false;
      _x = x; _y = y; _z = z;
    }
    else{
      _ownData = true;
      _x = new double[numNodes];
      _y = new double[numNodes];
      _z = new double[numNodes];
      for(int i = 0; i < numNodes; i++){
        _x[i] = x[i];
        _y[i] = y[i];
        _z[i] = z[i];
      }
    }
  }
  virtual ~element()
  {
    if(_ownData){
      delete [] _x;
      delete [] _y;
      delete [] _z;
    }
  }
  virtual void getXYZ(int num, double &x, double &y, double &z)
  {
    if(num < 0 || num >= getNumNodes()) return;
    x = _x[num];
    y = _y[num];
    z = _z[num];
  }
  static void setTolerance (const double tol){ TOL = tol; }
  static double getTolerance () { return TOL; }
  virtual int getDimension() = 0;
  virtual int getNumNodes() = 0;
  virtual void getNode(int num, double &u, double &v, double &w) = 0;
  virtual int getNumEdges() = 0;
  virtual void getEdge(int num, int &start, int &end) = 0;
  virtual int getNumGaussPoints() = 0;
  virtual void getGaussPoint(int num, double &u, double &v, double &w, double &weight) = 0;
  virtual void getShapeFunction(int num, double u, double v, double w, double &s) = 0;
  virtual void getGradShapeFunction(int num, double u, double v, double w, double s[3]) = 0;
  double getJacobian(double u, double v, double w, double jac[3][3])
  {
    jac[0][0] = jac[0][1] = jac[0][2] = 0.;
    jac[1][0] = jac[1][1] = jac[1][2] = 0.;
    jac[2][0] = jac[2][1] = jac[2][2] = 0.;
    double s[3];
    switch(getDimension()){
    case 3 :
      for(int i = 0; i < getNumNodes(); i++) {
        getGradShapeFunction(i, u, v, w, s);
        jac[0][0] += _x[i] * s[0]; jac[0][1] += _y[i] * s[0]; jac[0][2] += _z[i] * s[0];
        jac[1][0] += _x[i] * s[1]; jac[1][1] += _y[i] * s[1]; jac[1][2] += _z[i] * s[1];
        jac[2][0] += _x[i] * s[2]; jac[2][1] += _y[i] * s[2]; jac[2][2] += _z[i] * s[2];
      }
      return fabs(
        jac[0][0] * jac[1][1] * jac[2][2] + jac[0][2] * jac[1][0] * jac[2][1] +
        jac[0][1] * jac[1][2] * jac[2][0] - jac[0][2] * jac[1][1] * jac[2][0] -
        jac[0][0] * jac[1][2] * jac[2][1] - jac[0][1] * jac[1][0] * jac[2][2]);
    case 2 :
      for(int i = 0; i < getNumNodes(); i++) {
        getGradShapeFunction(i, u, v, w, s);
        jac[0][0] += _x[i] * s[0]; jac[0][1] += _y[i] * s[0]; jac[0][2] += _z[i] * s[0];
        jac[1][0] += _x[i] * s[1]; jac[1][1] += _y[i] * s[1]; jac[1][2] += _z[i] * s[1];
      }
      {
        double a[3], b[3], c[3];
        a[0]= _x[1] - _x[0]; a[1]= _y[1] - _y[0]; a[2]= _z[1] - _z[0];
        b[0]= _x[2] - _x[0]; b[1]= _y[2] - _y[0]; b[2]= _z[2] - _z[0];
        prodve(a, b, c);
        jac[2][0] = c[0]; jac[2][1] = c[1]; jac[2][2] = c[2];
      }
      return sqrt(gmsh_SQU(jac[0][0] * jac[1][1] - jac[0][1] * jac[1][0]) +
                  gmsh_SQU(jac[0][2] * jac[1][0] - jac[0][0] * jac[1][2]) +
                  gmsh_SQU(jac[0][1] * jac[1][2] - jac[0][2] * jac[1][1]));
    case 1:
      for(int i = 0; i < getNumNodes(); i++) {
        getGradShapeFunction(i, u, v, w, s);
        jac[0][0] += _x[i] * s[0]; jac[0][1] += _y[i] * s[0]; jac[0][2] += _z[i] * s[0];
      }
      {
        double a[3], b[3], c[3];
        a[0]= _x[1] - _x[0]; a[1]= _y[1] - _y[0]; a[2]= _z[1] - _z[0];
        if((fabs(a[0]) >= fabs(a[1]) && fabs(a[0]) >= fabs(a[2])) ||
           (fabs(a[1]) >= fabs(a[0]) && fabs(a[1]) >= fabs(a[2]))) {
          b[0] = a[1]; b[1] = -a[0]; b[2] = 0.;
        }
        else {
          b[0] = 0.; b[1] = a[2]; b[2] = -a[1];
        }
        prodve(a, b, c);
        jac[1][0] = b[0]; jac[1][1] = b[1]; jac[1][2] = b[2];
        jac[2][0] = c[0]; jac[2][1] = c[1]; jac[2][2] = c[2];
      }
      return sqrt(gmsh_SQU(jac[0][0])+gmsh_SQU(jac[0][1])+gmsh_SQU(jac[0][2]));
    default:
      jac[0][0] = jac[1][1] = jac[2][2] = 1.;
      return 1.;
    }
  }
  double interpolate(double val[], double u, double v, double w, int stride=1)
  {
    double sum = 0;
    int j = 0;
    for(int i = 0; i < getNumNodes(); i++){
      double s;
      getShapeFunction(i, u, v, w, s);
      sum += val[j] * s;
      j += stride;
    }
    return sum;
  }
  void interpolateGrad(double val[], double u, double v, double w, double f[3], int stride=1,
                       double invjac[3][3]=NULL)
  {
    double dfdu[3] = {0., 0., 0.};
    int j = 0;
    for(int i = 0; i < getNumNodes(); i++){
      double s[3];
      getGradShapeFunction(i, u, v, w, s);
      dfdu[0] += val[j] * s[0];
      dfdu[1] += val[j] * s[1];
      dfdu[2] += val[j] * s[2];
      j += stride;
    }
    if(invjac){
      matvec(invjac, dfdu, f);
    }
    else{
      double jac[3][3], inv[3][3];
      getJacobian(u, v, w, jac);
      inv3x3(jac, inv);
      matvec(inv, dfdu, f);
    }
  }
  void interpolateCurl(double val[], double u, double v, double w, double f[3], int stride=3)
  {
    double fx[3], fy[3], fz[3], jac[3][3], inv[3][3];
    getJacobian(u, v, w, jac);
    inv3x3(jac, inv);
    interpolateGrad(&val[0], u, v, w, fx, stride, inv);
    interpolateGrad(&val[1], u, v, w, fy, stride, inv);
    interpolateGrad(&val[2], u, v, w, fz, stride, inv);
    f[0] = fz[1] - fy[2];
    f[1] = -(fz[0] - fx[2]);
    f[2] = fy[0] - fx[1];
  }
  double interpolateDiv(double val[], double u, double v, double w, int stride=3)
  {
    double fx[3], fy[3], fz[3], jac[3][3], inv[3][3];
    getJacobian(u, v, w, jac);
    inv3x3(jac, inv);
    interpolateGrad(&val[0], u, v, w, fx, stride, inv);
    interpolateGrad(&val[1], u, v, w, fy, stride, inv);
    interpolateGrad(&val[2], u, v, w, fz, stride, inv);
    return fx[0] + fy[1] + fz[2];
  }
  double integrate(double val[], int stride=1)
  {
    double sum = 0;
    for(int i = 0; i < getNumGaussPoints(); i++){
      double u, v, w, weight, jac[3][3];
      getGaussPoint(i, u, v, w, weight);
      double det = getJacobian(u, v, w, jac);
      double d = interpolate(val, u, v, w, stride);
      sum += d * weight * det;
    }
    return sum;
  }
  double integrateLevelsetPositive(double val[])
  {
    // FIXME: explain + generalize this
    double ones[8] = {1., 1., 1., 1., 1., 1., 1., 1.};
    double area = integrate(ones);
    double sum = 0, sumabs = 0.;
    for(int i = 0; i < getNumNodes(); i++){
      sum += val[i];
      sumabs += fabs(val[i]);
    }
    double res = 0.;
    if(sumabs)
      res = area * (1 + sum/sumabs) * 0.5 ;
    return res;
  }
  virtual double integrateCirculation(double val[])
  {
    Msg::Error("integrateCirculation not available for this element");
    return 0.;
  }
  virtual double integrateFlux(double val[])
  {
    Msg::Error("integrateFlux not available for this element");
    return 0.;
  }
  virtual void xyz2uvw(double xyz[3], double uvw[3])
  {
    // general newton routine for the nonlinear case (more efficient
    // routines are implemented for simplices, where the basis
    // functions are linear)
    uvw[0] = uvw[1] = uvw[2] = 0.0;

    int iter = 1, maxiter = 20;
    double error = 1., tol = 1.e-6;

    while (error > tol && iter < maxiter){
      double jac[3][3];
      if(!getJacobian(uvw[0], uvw[1], uvw[2], jac)) break;

      double xn = 0., yn = 0., zn = 0.;
      for (int i = 0; i < getNumNodes(); i++) {
        double s;
        getShapeFunction(i, uvw[0], uvw[1], uvw[2], s);
        xn += _x[i] * s;
        yn += _y[i] * s;
        zn += _z[i] * s;
      }
      double inv[3][3];
      inv3x3(jac, inv);

      double un = uvw[0] +
        inv[0][0] * (xyz[0] - xn) + inv[1][0] * (xyz[1] - yn) + inv[2][0] * (xyz[2] - zn);
      double vn = uvw[1] +
        inv[0][1] * (xyz[0] - xn) + inv[1][1] * (xyz[1] - yn) + inv[2][1] * (xyz[2] - zn) ;
      double wn = uvw[2] +
        inv[0][2] * (xyz[0] - xn) + inv[1][2] * (xyz[1] - yn) + inv[2][2] * (xyz[2] - zn) ;

      error = sqrt(gmsh_SQU(un - uvw[0]) + gmsh_SQU(vn - uvw[1]) + gmsh_SQU(wn - uvw[2]));
      uvw[0] = un;
      uvw[1] = vn;
      uvw[2] = wn;
      iter++ ;
    }
    //if(error > tol) Msg::Warning("Newton did not converge in xyz2uvw") ;
  }
  virtual int isInside(double u, double v, double w) = 0;
  double maxEdgeLength()
  {
    double max = 0.;
    for(int i = 0; i < getNumEdges(); i++){
      int n1, n2;
      getEdge(i, n1, n2);
      double d = sqrt(gmsh_SQU(_x[n1]-_x[n2]) + gmsh_SQU(_y[n1]-_y[n2]) + gmsh_SQU(_z[n1]-_z[n2]));
      if(d > max) max = d;
    }
    return max;
  }
};

class point : public element{
public:
  point(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 0; }
  inline int getNumNodes(){ return 1; }
  void getNode(int num, double &u, double &v, double &w)
  {
    u = v = w = 0.;
  }
  inline int getNumEdges(){ return 0; }
  void getEdge(int num, int &start, int &end)
  {
    start = end = 0;
  }
  inline int getNumGaussPoints(){ return 1; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    u = v = w = 0.;
    weight = 1.;
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = 1.; break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    s[0] = s[1] = s[2] = 0.;
  }
  void xyz2uvw(double xyz[3], double uvw[3])
  {
    uvw[0] = uvw[1] = uvw[2] = 0.;
  }
  int isInside(double u, double v, double w)
  {
    if(fabs(u) > TOL || fabs(v) > TOL || fabs(w) > TOL)
      return 0;
    return 1;
  }
};

class line : public element{
public:
  line(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 1; }
  inline int getNumNodes(){ return 2; }
  void getNode(int num, double &u, double &v, double &w)
  {
    v = w = 0.;
    switch(num) {
    case 0 : u = -1.; break;
    case 1 : u =  1.; break;
    default: u =  0.; break;
    }
  }
  inline int getNumEdges(){ return 1; }
  void getEdge(int num, int &start, int &end)
  {
    start = 0; end = 1;
  }
  inline int getNumGaussPoints(){ return 1; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    if(num < 0 || num > 0) return;
    u = v = w = 0.;
    weight = 2.;
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = 0.5 * (1.-u); break;
    case 1  : s = 0.5 * (1.+u); break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -0.5; s[1] = 0.; s[2] = 0.; break;
    case 1  : s[0] =  0.5; s[1] = 0.; s[2] = 0.; break;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  double integrateCirculation(double val[])
  {
    double t[3] = {_x[1]-_x[0], _y[1]-_y[0], _z[1]-_z[0]};
    norme(t);
    double v[3];
    for(int i = 0; i < 3; i++)
      v[i] = integrate(&val[i], 3);
    double d;
    prosca(t, v, &d);
    return d;
  }
  int isInside(double u, double v, double w)
  {
    if(u < -(1. + TOL) || u > (1. + TOL) || fabs(v) > TOL || fabs(w) > TOL)
      return 0;
    return 1;
  }
};

class triangle : public element{
public:
  triangle(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 2; }
  inline int getNumNodes(){ return 3; }
  void getNode(int num, double &u, double &v, double &w)
  {
    w = 0.;
    switch(num) {
    case 0 : u = 0.; v = 0.; break;
    case 1 : u = 1.; v = 0.; break;
    case 2 : u = 0.; v = 1.; break;
    default: u = 0.; v = 0.; break;
    }
  }
  inline int getNumEdges(){ return 3; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 1; end = 2; break;
    case 2 : start = 2; end = 0; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return /* 3 */ 1; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    /*
      static double u3[3] = {0.16666666666666,0.66666666666666,0.16666666666666};
      static double v3[3] = {0.16666666666666,0.16666666666666,0.66666666666666};
      static double p3[3] = {0.16666666666666,0.16666666666666,0.16666666666666};
      if(num < 0 || num > 2) return;
      u = u3[num];
      v = v3[num];
      w = 0.;
      weight = p3[num];
    */
    if(num < 0 || num > 0) return;
    u = v = 0.333333333333333; w = 0.;
    weight = 0.5;
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num){
    case 0  : s = 1.-u-v; break;
    case 1  : s =    u  ; break;
    case 2  : s =      v; break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -1.; s[1] = -1.; s[2] =  0.; break;
    case 1  : s[0] =  1.; s[1] =  0.; s[2] =  0.; break;
    case 2  : s[0] =  0.; s[1] =  1.; s[2] =  0.; break;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  double integrateFlux(double val[])
  {
    double t1[3] = {_x[1]-_x[0], _y[1]-_y[0], _z[1]-_z[0]};
    double t2[3] = {_x[2]-_x[0], _y[2]-_y[0], _z[2]-_z[0]};
    double n[3];
    prodve(t1, t2, n);
    norme(n);
    double v[3];
    for(int i = 0; i < 3; i++)
      v[i] = integrate(&val[i], 3);
    double d;
    prosca(n, v, &d);
    return d;
  }
#if 0 // faster, but only valid for triangles in the z=0 plane
  void xyz2uvw(double xyz[3], double uvw[3])
  {
    double mat[2][2], b[2];
    mat[0][0] = _x[1] - _x[0];
    mat[0][1] = _x[2] - _x[0];
    mat[1][0] = _y[1] - _y[0];
    mat[1][1] = _y[2] - _y[0];
    b[0] = xyz[0] - _x[0];
    b[1] = xyz[1] - _y[0];
    sys2x2(mat, b, uvw);
    uvw[2] = 0.;
  }
#endif
  int isInside(double u, double v, double w)
  {
    if(u < -TOL || v < -TOL || u > ((1. + TOL) - v) || fabs(w) > TOL)
      return 0;
    return 1;
  }
};

class quadrangle : public element{
public:
  quadrangle(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 2; }
  inline int getNumNodes(){ return 4; }
  void getNode(int num, double &u, double &v, double &w)
  {
    w = 0.;
    switch(num) {
    case 0 : u = -1.; v = -1.; break;
    case 1 : u =  1.; v = -1.; break;
    case 2 : u =  1.; v =  1.; break;
    case 3 : u = -1.; v =  1.; break;
    default: u =  0.; v =  0.; break;
    }
  }
  inline int getNumEdges(){ return 4; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 1; end = 2; break;
    case 2 : start = 2; end = 3; break;
    case 3 : start = 3; end = 0; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return 4; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    static double u4[4] = {0.577350269189,-0.577350269189,0.577350269189,-0.577350269189};
    static double v4[4] = {0.577350269189,0.577350269189,-0.577350269189,-0.577350269189};
    static double p4[4] = {1.,1.,1.,1.};
    if(num < 0 || num > 3) return;
    u = u4[num];
    v = v4[num];
    w = 0.;
    weight = p4[num];
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = 0.25 * (1.-u) * (1.-v); break;
    case 1  : s = 0.25 * (1.+u) * (1.-v); break;
    case 2  : s = 0.25 * (1.+u) * (1.+v); break;
    case 3  : s = 0.25 * (1.-u) * (1.+v); break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -0.25 * (1.-v); s[1] = -0.25 * (1.-u); s[2] = 0.; break;
    case 1  : s[0] =  0.25 * (1.-v); s[1] = -0.25 * (1.+u); s[2] = 0.; break;
    case 2  : s[0] =  0.25 * (1.+v); s[1] =  0.25 * (1.+u); s[2] = 0.; break;
    case 3  : s[0] = -0.25 * (1.+v); s[1] =  0.25 * (1.-u); s[2] = 0.; break;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  double integrateFlux(double val[])
  {
    double t1[3] = {_x[1]-_x[0], _y[1]-_y[0], _z[1]-_z[0]};
    double t2[3] = {_x[2]-_x[0], _y[2]-_y[0], _z[2]-_z[0]};
    double n[3];
    prodve(t1, t2, n);
    norme(n);
    double v[3];
    for(int i = 0; i < 3; i++)
      v[i] = integrate(&val[i], 3);
    double d;
    prosca(n, v, &d);
    return d;
  }
  int isInside(double u, double v, double w)
  {
    if(u < -(1. + TOL) || v < -(1. + TOL) || u > (1. + TOL) || v > (1. + TOL) ||
       fabs(w) > TOL)
      return 0;
    return 1;
  }
};

class tetrahedron : public element{
public:
  tetrahedron(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 3; }
  inline int getNumNodes(){ return 4; }
  void getNode(int num, double &u, double &v, double &w)
  {
    switch(num) {
    case 0 : u = 0.; v = 0.; w = 0.; break;
    case 1 : u = 1.; v = 0.; w = 0.; break;
    case 2 : u = 0.; v = 1.; w = 0.; break;
    case 3 : u = 0.; v = 0.; w = 1.; break;
    default: u = 0.; v = 0.; w = 0.; break;
    }
  }
  inline int getNumEdges(){ return 6; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 1; end = 2; break;
    case 2 : start = 2; end = 0; break;
    case 3 : start = 3; end = 0; break;
    case 4 : start = 3; end = 2; break;
    case 5 : start = 3; end = 1; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return 4; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    static double u4[4] = {0.138196601125,0.138196601125,0.138196601125,0.585410196625};
    static double v4[4] = {0.138196601125,0.138196601125,0.585410196625,0.138196601125};
    static double w4[4] = {0.138196601125,0.585410196625,0.138196601125,0.138196601125};
    static double p4[4] = {0.0416666666667,0.0416666666667,0.0416666666667,0.0416666666667};
    if(num < 0 || num > 3) return;
    u = u4[num];
    v = v4[num];
    w = w4[num];
    weight = p4[num];
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = 1.-u-v-w; break;
    case 1  : s =    u    ; break;
    case 2  : s =      v  ; break;
    case 3  : s =        w; break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -1.; s[1] = -1.; s[2] = -1.; break;
    case 1  : s[0] =  1.; s[1] =  0.; s[2] =  0.; break;
    case 2  : s[0] =  0.; s[1] =  1.; s[2] =  0.; break;
    case 3  : s[0] =  0.; s[1] =  0.; s[2] =  1.; break;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  void xyz2uvw(double xyz[3], double uvw[3])
  {
    double mat[3][3], b[3];
    mat[0][0] = _x[1] - _x[0];
    mat[0][1] = _x[2] - _x[0];
    mat[0][2] = _x[3] - _x[0];
    mat[1][0] = _y[1] - _y[0];
    mat[1][1] = _y[2] - _y[0];
    mat[1][2] = _y[3] - _y[0];
    mat[2][0] = _z[1] - _z[0];
    mat[2][1] = _z[2] - _z[0];
    mat[2][2] = _z[3] - _z[0];
    b[0] = xyz[0] - _x[0];
    b[1] = xyz[1] - _y[0];
    b[2] = xyz[2] - _z[0];
    double det;
    sys3x3(mat, b, uvw, &det);
  }
  int isInside(double u, double v, double w)
  {
    if(u < (-TOL) || v < (-TOL) || w < (-TOL) || u > ((1. + TOL) - v - w))
      return 0;
    return 1;
  }
};

class hexahedron : public element{
public:
  hexahedron(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 3; }
  inline int getNumNodes(){ return 8; }
  void getNode(int num, double &u, double &v, double &w)
  {
    switch(num) {
    case 0 : u = -1.; v = -1.; w = -1.; break;
    case 1 : u =  1.; v = -1.; w = -1.; break;
    case 2 : u =  1.; v =  1.; w = -1.; break;
    case 3 : u = -1.; v =  1.; w = -1.; break;
    case 4 : u = -1.; v = -1.; w =  1.; break;
    case 5 : u =  1.; v = -1.; w =  1.; break;
    case 6 : u =  1.; v =  1.; w =  1.; break;
    case 7 : u = -1.; v =  1.; w =  1.; break;
    default: u =  0.; v =  0.; w =  0.; break;
    }
  }
  inline int getNumEdges(){ return 12; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 0; end = 3; break;
    case 2 : start = 0; end = 4; break;
    case 3 : start = 1; end = 2; break;
    case 4 : start = 1; end = 5; break;
    case 5 : start = 2; end = 3; break;
    case 6 : start = 2; end = 6; break;
    case 7 : start = 3; end = 7; break;
    case 8 : start = 4; end = 5; break;
    case 9 : start = 4; end = 7; break;
    case 10: start = 5; end = 6; break;
    case 11: start = 6; end = 7; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return 6; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    static double u6[6] = { 0.40824826,  0.40824826, -0.40824826,
                            -0.40824826, -0.81649658,  0.81649658};
    static double v6[6] = { 0.70710678, -0.70710678,  0.70710678,
                            -0.70710678,  0.,  0.};
    static double w6[6] = {-0.57735027, -0.57735027,  0.57735027,
                           0.57735027, -0.57735027,  0.57735027};
    static double p6[6] = { 1.3333333333,  1.3333333333,  1.3333333333,
                            1.3333333333,  1.3333333333,  1.3333333333};
    if(num < 0 || num > 5) return;
    u = u6[num];
    v = v6[num];
    w = w6[num];
    weight = p6[num];
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = (1.-u) * (1.-v) * (1.-w) * 0.125; break;
    case 1  : s = (1.+u) * (1.-v) * (1.-w) * 0.125; break;
    case 2  : s = (1.+u) * (1.+v) * (1.-w) * 0.125; break;
    case 3  : s = (1.-u) * (1.+v) * (1.-w) * 0.125; break;
    case 4  : s = (1.-u) * (1.-v) * (1.+w) * 0.125; break;
    case 5  : s = (1.+u) * (1.-v) * (1.+w) * 0.125; break;
    case 6  : s = (1.+u) * (1.+v) * (1.+w) * 0.125; break;
    case 7  : s = (1.-u) * (1.+v) * (1.+w) * 0.125; break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -0.125 * (1.-v) * (1.-w);
              s[1] = -0.125 * (1.-u) * (1.-w);
              s[2] = -0.125 * (1.-u) * (1.-v); break;
    case 1  : s[0] =  0.125 * (1.-v) * (1.-w);
              s[1] = -0.125 * (1.+u) * (1.-w);
              s[2] = -0.125 * (1.+u) * (1.-v); break;
    case 2  : s[0] =  0.125 * (1.+v) * (1.-w);
              s[1] =  0.125 * (1.+u) * (1.-w);
              s[2] = -0.125 * (1.+u) * (1.+v); break;
    case 3  : s[0] = -0.125 * (1.+v) * (1.-w);
              s[1] =  0.125 * (1.-u) * (1.-w);
              s[2] = -0.125 * (1.-u) * (1.+v); break;
    case 4  : s[0] = -0.125 * (1.-v) * (1.+w);
              s[1] = -0.125 * (1.-u) * (1.+w);
              s[2] =  0.125 * (1.-u) * (1.-v); break;
    case 5  : s[0] =  0.125 * (1.-v) * (1.+w);
              s[1] = -0.125 * (1.+u) * (1.+w);
              s[2] =  0.125 * (1.+u) * (1.-v); break;
    case 6  : s[0] =  0.125 * (1.+v) * (1.+w);
              s[1] =  0.125 * (1.+u) * (1.+w);
              s[2] =  0.125 * (1.+u) * (1.+v); break;
    case 7  : s[0] = -0.125 * (1.+v) * (1.+w);
              s[1] =  0.125 * (1.-u) * (1.+w);
              s[2] =  0.125 * (1.-u) * (1.+v); break;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  int isInside(double u, double v, double w)
  {
    if(u < -(1. + TOL) || v < -(1. + TOL) || w < -(1. + TOL) ||
       u > (1. + TOL) || v > (1. + TOL) || w > (1. + TOL))
      return 0;
    return 1;
  }
};

class prism : public element{
public:
  prism(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 3; }
  inline int getNumNodes(){ return 6; }
  void getNode(int num, double &u, double &v, double &w)
  {
    switch(num) {
    case 0 : u = 0.; v = 0.; w = -1.; break;
    case 1 : u = 1.; v = 0.; w = -1.; break;
    case 2 : u = 0.; v = 1.; w = -1.; break;
    case 3 : u = 0.; v = 0.; w =  1.; break;
    case 4 : u = 1.; v = 0.; w =  1.; break;
    case 5 : u = 0.; v = 1.; w =  1.; break;
    default: u = 0.; v = 0.; w =  0.; break;
    }
  }
  inline int getNumEdges(){ return 9; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 0; end = 2; break;
    case 2 : start = 0; end = 3; break;
    case 3 : start = 1; end = 2; break;
    case 4 : start = 1; end = 4; break;
    case 5 : start = 2; end = 5; break;
    case 6 : start = 3; end = 4; break;
    case 7 : start = 3; end = 5; break;
    case 8 : start = 4; end = 5; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return 6; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    static double u6[6] = {0.166666666666666, 0.333333333333333, 0.166666666666666,
                           0.166666666666666, 0.333333333333333, 0.166666666666666};
    static double v6[6] = {0.166666666666666, 0.166666666666666, 0.333333333333333,
                           0.166666666666666, 0.166666666666666, 0.333333333333333};
    static double w6[6] = {-0.577350269189, -0.577350269189, -0.577350269189,
                           0.577350269189,  0.577350269189,  0.577350269189};
    static double p6[6] = {0.166666666666666,0.166666666666666,0.166666666666666,
                           0.166666666666666,0.166666666666666,0.166666666666666,};
    if(num < 0 || num > 5) return;
    u = u6[num];
    v = v6[num];
    w = w6[num];
    weight = p6[num];
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    switch(num) {
    case 0  : s = (1.-u-v) * (1.-w) * 0.5; break;
    case 1  : s =     u    * (1.-w) * 0.5; break;
    case 2  : s =       v  * (1.-w) * 0.5; break;
    case 3  : s = (1.-u-v) * (1.+w) * 0.5; break;
    case 4  : s =     u    * (1.+w) * 0.5; break;
    case 5  : s =       v  * (1.+w) * 0.5; break;
    default : s = 0.; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    switch(num) {
    case 0  : s[0] = -0.5 * (1.-w)   ;
              s[1] = -0.5 * (1.-w)   ;
              s[2] = -0.5 * (1.-u-v) ; break ;
    case 1  : s[0] =  0.5 * (1.-w)   ;
              s[1] =  0.             ;
              s[2] = -0.5 * u        ; break ;
    case 2  : s[0] =  0.             ;
              s[1] =  0.5 * (1.-w)   ;
              s[2] = -0.5 * v        ; break ;
    case 3  : s[0] = -0.5 * (1.+w)   ;
              s[1] = -0.5 * (1.+w)   ;
              s[2] =  0.5 * (1.-u-v) ; break ;
    case 4  : s[0] =  0.5 * (1.+w)   ;
              s[1] =  0.             ;
              s[2] =  0.5 * u        ; break ;
    case 5  : s[0] =  0.             ;
              s[1] =  0.5 * (1.+w)   ;
              s[2] =  0.5 * v        ; break ;
    default : s[0] = s[1] = s[2] = 0.; break;
    }
  }
  int isInside(double u, double v, double w)
  {
    if(w > (1. + TOL) || w < -(1. + TOL) || u < (-TOL) || v < (-TOL) || u > ((1. + TOL) - v))
      return 0;
    return 1;
  }
};

class pyramid : public element{
public:
  pyramid(double *x, double *y, double *z, int numNodes=0) : element(x, y, z, numNodes) {}
  inline int getDimension(){ return 3; }
  inline int getNumNodes(){ return 5; }
  void getNode(int num, double &u, double &v, double &w)
  {
    switch(num) {
    case 0 : u = -1.; v = -1.; w = 0.; break;
    case 1 : u =  1.; v = -1.; w = 0.; break;
    case 2 : u =  1.; v =  1.; w = 0.; break;
    case 3 : u = -1.; v =  1.; w = 0.; break;
    case 4 : u =  0.; v =  0.; w = 1.; break;
    default: u =  0.; v =  0.; w = 0.; break;
    }
  }
  inline int getNumEdges(){ return 8; }
  void getEdge(int num, int &start, int &end)
  {
    switch(num) {
    case 0 : start = 0; end = 1; break;
    case 1 : start = 0; end = 3; break;
    case 2 : start = 0; end = 4; break;
    case 3 : start = 1; end = 2; break;
    case 4 : start = 1; end = 4; break;
    case 5 : start = 2; end = 3; break;
    case 6 : start = 2; end = 4; break;
    case 7 : start = 3; end = 4; break;
    default: start = end = 0; break;
    }
  }
  inline int getNumGaussPoints(){ return 8; }
  void getGaussPoint(int num, double &u, double &v, double &w, double &weight)
  {
    static double u8[8] = {0.2631840555694285,-0.2631840555694285,
                           0.2631840555694285,-0.2631840555694285,
                           0.5066163033492386,-0.5066163033492386,
                           0.5066163033492386,-0.5066163033492386};
    static double v8[8] = {0.2631840555694285,0.2631840555694285,
                           -0.2631840555694285,-0.2631840555694285,
                           0.5066163033492386,0.5066163033492386,
                           -0.5066163033492386,-0.5066163033492386};
    static double w8[8] = {0.544151844011225,0.544151844011225,
                           0.544151844011225,0.544151844011225,
                           0.122514822655441,0.122514822655441,
                           0.122514822655441,0.122514822655441};
    static double p8[8] = {0.100785882079825,0.100785882079825,
                           0.100785882079825,0.100785882079825,
                           0.232547451253508,0.232547451253508,
                           0.232547451253508,0.232547451253508};
    if(num < 0 || num > 7) return;
    u = u8[num];
    v = v8[num];
    w = w8[num];
    weight = p8[num];
  }
  void getShapeFunction(int num, double u, double v, double w, double &s)
  {
    double r;

    if(w != 1. && num != 4) r = u*v*w / (1.-w);
    else                    r = 0.;
    switch(num) {
    case 0  : s = 0.25 * ((1.-u) * (1.-v) - w + r); break;
    case 1  : s = 0.25 * ((1.+u) * (1.-v) - w - r); break;
    case 2  : s = 0.25 * ((1.+u) * (1.+v) - w + r); break;
    case 3  : s = 0.25 * ((1.-u) * (1.+v) - w - r); break;
    case 4  : s = w  ; break;
    default : s = 0. ; break;
    }
  }
  void getGradShapeFunction(int num, double u, double v, double w, double s[3])
  {
    if(w == 1.) {
      switch(num) {
      case 0 : s[0] = -0.25 ;
               s[1] = -0.25 ;
               s[2] = -0.25 ; break ;
      case 1 : s[0] =  0.25 ;
               s[1] = -0.25 ;
               s[2] = -0.25 ; break ;
      case 2 : s[0] =  0.25 ;
               s[1] =  0.25 ;
               s[2] = -0.25 ; break ;
      case 3 : s[0] = -0.25 ;
               s[1] =  0.25 ;
               s[2] = -0.25 ; break ;
      case 4 : s[0] =  0. ;
               s[1] =  0. ;
               s[2] =  1. ; break ;
      default : s[0] = s[1] = s[2] = 0.; break;
      }
    }
    else{
      switch(num) {
      case 0  : s[0] = 0.25 * ( -(1.-v) + v*w/(1.-w) ) ;
                s[1] = 0.25 * ( -(1.-u) + u*w/(1.-w) ) ;
                s[2] = 0.25 * ( -1.     + u*v/(1.-w) + u*v*w/gmsh_SQU(1.-w) ) ; break ;
      case 1  : s[0] = 0.25 * (  (1.-v) - v*w/(1.-w) ) ;
                s[1] = 0.25 * ( -(1.+u) - u*w/(1.-w) ) ;
                s[2] = 0.25 * ( -1.     - u*v/(1.-w) - u*v*w/gmsh_SQU(1.-w) ) ; break ;
      case 2  : s[0] = 0.25 * (  (1.+v) + v*w/(1.-w) ) ;
                s[1] = 0.25 * (  (1.+u) + u*w/(1.-w) ) ;
                s[2] = 0.25 * ( -1.     + u*v/(1.-w) + u*v*w/gmsh_SQU(1.-w) ) ; break ;
      case 3  : s[0] = 0.25 * ( -(1.+v) - v*w/(1.-w) ) ;
                s[1] = 0.25 * (  (1.-u) - u*w/(1.-w) ) ;
                s[2] = 0.25 * ( -1.     - u*v/(1.-w) - u*v*w/gmsh_SQU(1.-w) ) ; break ;
      case 4  : s[0] = 0. ;
                s[1] = 0. ;
                s[2] = 1. ; break ;
      default : s[0] = s[1] = s[2] = 0.; break;
      }
    }
  }
  int isInside(double u, double v, double w)
  {
    if(u < (w - (1. + TOL)) || u > ((1. + TOL) - w) || v < (w - (1. + TOL)) ||
       v > ((1. + TOL) - w) || w < (-TOL) || w > (1. + TOL))
      return 0;
    return 1;
  }
};

class elementFactory{
 public:
  element* create(int numNodes, int dimension, double *x, double *y, double *z,
                  bool copy=false)
  {
    switch(dimension){
    case 0: return new point(x, y, z, copy ? numNodes : 0);
    case 1: return new line(x, y, z, copy ? numNodes : 0);
    case 2:
      if(numNodes == 4) return new quadrangle(x, y, z, copy ? numNodes : 0);
      else return new triangle(x, y, z, copy ? numNodes : 0);
    case 3:
      if(numNodes == 8) return new hexahedron(x, y, z, copy ? numNodes : 0);
      else if(numNodes == 6) return new prism(x, y, z, copy ? numNodes : 0);
      else if(numNodes == 5) return new pyramid(x, y, z, copy ? numNodes : 0);
      else return new tetrahedron(x, y, z, copy ? numNodes : 0);
    default:
      Msg::Error("Unknown type of element in factory");
      return NULL;
    }
  }
};

#undef SQU

#endif
