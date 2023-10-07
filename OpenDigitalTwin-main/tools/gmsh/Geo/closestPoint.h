// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _CLOSEST_POINT_H_
#define _CLOSEST_POINT_H_

#include "GmshConfig.h"

#if defined(HAVE_ANN)
#include "ANN/ANN.h"
#endif

#include "SPoint3.h"

class GEntity;
class closestPointFinder
{
#if defined(HAVE_ANN)
  ANNkd_tree *kdtree;
  ANNpointArray zeronodes;
  ANNidxArray index;
  ANNdistArray dist;
#endif
  double _tolerance;
 public :
  closestPointFinder (GEntity*, double);
  ~closestPointFinder ();
  SPoint3 operator () (const SPoint3 & p);
  inline double tol() const {return _tolerance;}
};

#endif
