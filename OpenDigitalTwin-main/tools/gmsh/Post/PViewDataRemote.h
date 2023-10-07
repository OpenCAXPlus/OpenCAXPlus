// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _PVIEW_DATA_REMOTE_H_
#define _PVIEW_DATA_REMOTE_H_

#include <vector>
#include <string>
#include "GmshMessage.h"
#include "PViewData.h"
#include "SBoundingBox3d.h"
#include "onelab.h"

// The container for a remote dataset (does not contain any actual
// data)
class PViewDataRemote : public PViewData {
 private:
  onelab::localNetworkClient *_remote;
  double _min, _max;
  int _numTimeSteps;
  double _time;
  SBoundingBox3d _bbox;
 public:
  PViewDataRemote(onelab::localNetworkClient *remote, double min, double max,
                  int numsteps, double time, SBoundingBox3d &bbox)
    : _remote(remote), _min(min), _max(max), _numTimeSteps(numsteps),
      _time(time), _bbox(bbox) {}
  ~PViewDataRemote(){}
  bool finalize(bool computeMinMax=true, const std::string &interpolationScheme="")
  {
    return true;
  }
  int getNumTimeSteps(){ return _numTimeSteps; }
  double getMin(int step=-1, bool onlyVisible=false, int forceNumComponents=0,
                int componentMap[9]=0){ return _min; }
  double getMax(int step=-1, bool onlyVisible=false, int forceNumComponents=0,
                int componentMap[9]=0){ return _max; }
  SBoundingBox3d getBoundingBox(int step=-1){ return _bbox; }
  double getTime(int step){ return _time; }
  // need to return != 0 for "empty" tests
  int getNumElements(int step=-1, int ent=-1){ return -1; }
  void setMin(double min){ _min = min; }
  void setMax(double max){ _max = max; }
  void setBoundingBox(SBoundingBox3d &bbox){ _bbox = bbox; }
  void setTime(double time){ _time = time; }
  bool isRemote(){ return true; }
  int fillRemoteVertexArrays(std::string &options)
  {
    GmshServer *server = _remote->getGmshServer();
    if(!server){
      Msg::Error("Remote server not running: please start server");
      return 1;
    }
    setDirty(true);
    server->SendString(GmshSocket::GMSH_VERTEX_ARRAY, options.c_str());
    return 1;
  }
};

#endif
