// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _ROBUST_PREDICATES_H_
#define _ROBUST_PREDICATES_H_

// namespace necessary to avoid conflicts with predicates used by Tetgen
namespace robustPredicates{
  double exactinit(int filter, double maxx, double maxy, double maxz);
  double incircle(double *pa, double *pb, double *pc, double *pd);
  double insphere(double *pa, double *pb, double *pc, double *pd, double *pe);
  double orient2d(double *pa, double *pb, double *pc);
  double orient3d(double *pa, double *pb, double *pc, double *pd);
}

#endif
