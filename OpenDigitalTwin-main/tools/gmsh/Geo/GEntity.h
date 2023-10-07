// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GENTITY_H_
#define _GENTITY_H_

#include <list>
#include <string>
#include <vector>
#include "Range.h"
#include "SPoint3.h"
#include "SBoundingBox3d.h"
#include "SOrientedBoundingBox.h"

#define MAX_LC 1.e22

class GModel;
class GVertex;
class GEdge;
class GFace;
class GRegion;
class MVertex;
class MElement;
class VertexArray;

// A geometric model entity.
class GEntity {
 private:
  // all entities are owned by a GModel
  GModel *_model;

  // the tag (the number) of this entity
  int _tag;

  // gives the number of the master entity in periodic mesh, gives _tag
  // if non-periodic
  GEntity *_meshMaster;

  // the visibility and the selection flag
  char _visible, _selection;

  // flag storing if all mesh elements are visible
  char _allElementsVisible;

  // the color of the entity (ignored if set to transparent blue)
  unsigned int _color;

 protected:
  SOrientedBoundingBox *_obb;

 public: // these will become protected at some point
  // the mesh vertices uniquely owned by the entity
  std::vector<MVertex*> mesh_vertices;

  // a list of geometrical entities that form a compound mesh
  std::vector<GEntity *> _compound;

  // corresponding principal vertices
  std::map<GVertex*,GVertex*> vertexCounterparts;

  // the physical entitites (if any) that contain this entity
  std::vector<int> physicals;

  // vertex arrays to draw the mesh efficiently
  VertexArray *va_lines, *va_triangles;

 public:
  // all known native model types
  enum ModelType {
    UnknownModel,
    GmshModel,
    FourierModel,
    OpenCascadeModel,
    GenericModel,
    AcisModel
  };

  // all known entity types
  enum GeomType {
    Unknown,
    Point,
    BoundaryLayerPoint,
    Line,
    Circle,
    Ellipse,
    Conic,
    Parabola,
    Hyperbola,
    TrimmedCurve,
    OffsetCurve,
    BSpline,
    Bezier,
    ParametricCurve,
    BoundaryLayerCurve,
    CompoundCurve,
    DiscreteCurve,
    Plane,
    Nurb,
    Cylinder,
    Sphere,
    Cone,
    Torus,
    RuledSurface,
    ParametricSurface,
    ProjectionFace,
    BSplineSurface,
    BezierSurface,
    SurfaceOfRevolution,
    BoundaryLayerSurface,
    DiscreteSurface,
    DiscreteDiskSurface,
    CompoundSurface,
    Volume,
    DiscreteVolume,
    CompoundVolume,
    PartitionVertex,
    PartitionCurve,
    PartitionSurface
  };

  enum MeshGenerationStatus {
    PENDING,
    DONE,
    FAILED
  };

  // return a string describing the entity type
  virtual std::string getTypeString()
  {
    const char *name[] = {
      "Unknown",
      "Point",
      "Boundary layer point",
      "Line",
      "Circle",
      "Ellipse",
      "Conic",
      "Parabola",
      "Hyperbola",
      "TrimmedCurve",
      "OffsetCurve",
      "BSpline",
      "Bezier",
      "Parametric curve",
      "Boundary layer curve",
      "Compound curve",
      "Discrete curve",
      "Plane",
      "Nurb",
      "Cylinder",
      "Sphere",
      "Cone",
      "Torus",
      "Ruled surface",
      "Parametric surface",
      "Projection surface",
      "BSpline surface",
      "Bezier surface",
      "Surface of Revolution",
      "Boundary layer surface",
      "Discrete surface",
      "Discrete surface (parametrizable, isomorphic to a disk)",
      "Compound surface",
      "Volume",
      "Discrete volume",
      "Compound Volume",
      "Partition vertex",
      "Partition curve",
      "Partition surface"
    };
    unsigned int type = (unsigned int)geomType();
    if(type >= sizeof(name) / sizeof(name[0]))
      return "Undefined";
    else
      return name[type];
  }

  GEntity(GModel *m,int t);

  virtual ~GEntity(){}

  // mesh generation of the entity
  virtual void mesh(bool verbose) {}

  // delete the mesh data
  virtual void deleteMesh(){}

  // delete the vertex arrays, used to to draw the mesh efficiently
  void deleteVertexArrays();

  // spatial dimension of the entity
  virtual int dim() const { return -1; }

  // regions that bound this entity or that this entity bounds.
  virtual std::list<GRegion*> regions() const { return std::list<GRegion*>(); }

  // faces that bound this entity or that this entity bounds.
  virtual std::list<GFace*> faces() const { return std::list<GFace*>(); }

  // edges that bound this entity or that this entity bounds.
  virtual std::list<GEdge*> edges() const { return std::list<GEdge*>(); }

  // vertices that bound this entity.
  virtual std::list<GVertex*> vertices() const { return std::list<GVertex*>(); }

  // for Python, temporary solution while iterator are not binded
  std::vector<GRegion*> bindingsGetRegions()
  {
    // NOTE: two-line to not create two different lists with diff pointers
    std::list<GRegion*> r = regions();
    return std::vector<GRegion*> (r.begin(), r.end());
  }
  std::vector<GFace*> bindingsGetFaces()
  {
    std::list<GFace*> f = faces();
    return std::vector<GFace*> (f.begin(), f.end());
  }
  std::vector<GEdge*> bindingsGetEdges()
  {
    std::list<GEdge*> e = edges();
    return std::vector<GEdge*> (e.begin(), e.end());
  }
  std::vector<GVertex*> bindingsGetVertices()
  {
    std::list<GVertex*> v = vertices();
    return std::vector<GVertex*> (v.begin(), v.end());
  }

  // underlying geometric representation of this entity.
  virtual GeomType geomType() const { return Unknown; }

  // true if parametric space is continuous in the "dim" direction.
  virtual bool continuous(int dim) const { return true; }

  // true if entity is periodic in the "dim" direction.
  virtual bool periodic(int dim) const { return false; }
  virtual double period(int dim) const { return 0.0; }

  // true if there are parametric degeneracies in the "dim" direction.
  virtual bool degenerate(int dim) const { return false; }

  // does the entity have a parametrization?
  virtual bool haveParametrization(){ return true; }

  // parametric bounds of the entity in the "i" direction.
  virtual Range<double> parBounds(int i) const { return Range<double>(0., 0.); }

  // modeler tolerance for the entity.
  virtual double tolerance() const { return 1.e-14; }

  // true if the entity contains the given point to within tolerance.
  virtual bool containsPoint(const SPoint3 &pt) const { return false; }

  // get the native type of the particular representation
  virtual ModelType getNativeType() const { return UnknownModel; }

  // get the native pointer of the particular representation
  virtual void *getNativePtr() const { return 0; }

  // get the native id (int) of the particular representation
  virtual int getNativeInt() const { return 0; }

  // the model owning this entity
  GModel *model() const { return _model; }

  // get/set the tag of the entity
  int tag() const { return _tag; }
  void setTag(int tag) { _tag = tag; }

  // get/set physical entities
  virtual void addPhysicalEntity(int physicalTag)
  {
    physicals.push_back(physicalTag);
  }
  virtual std::vector<int> getPhysicalEntities()
  {
    return physicals;
  }

  // returns the master entity (for mesh)
  GEntity* meshMaster() const;
  void setMeshMaster(GEntity*);
  void setMeshMaster(GEntity*,const std::vector<double>&);

  // get the bounding box
  virtual SBoundingBox3d bounds() const { return SBoundingBox3d(); }

  //  get the oriented bounding box
  virtual SOrientedBoundingBox getOBB() {return SOrientedBoundingBox(); }

  // get/set the visibility flag
  virtual char getVisibility();
  virtual void setVisibility(char val, bool recursive=false){ _visible = val; }

  // get/set the selection flag
  virtual char getSelection(){ return _selection; }
  virtual void setSelection(char val){ _selection = val; }

  // get/set the color
  virtual unsigned int getColor(){ return _color; }
  virtual void setColor(unsigned color, bool recursive=false){ _color = color; }

  // return true if we should use this color to represent the entity
  virtual bool useColor();

  // return an information string for the entity
  virtual std::string getInfoString();

  // return a type-specific additional information string
  virtual std::string getAdditionalInfoString() { return std::string(""); }

  // reset the mesh attributes to default values
  virtual void resetMeshAttributes() { return; }

  // global mesh size constraint for the entity
  virtual double getMeshSize() const { return MAX_LC; }

  // number of types of elements
  virtual int getNumElementTypes() const { return 0; }

  // get the number of mesh elements (total and by type) in the entity
  virtual unsigned int getNumMeshElements() { return 0; }
  virtual unsigned int getNumMeshParentElements() { return 0; }
  virtual void getNumMeshElements(unsigned *const c) const { }

  // get the start of the array of a type of element
  virtual MElement *const *getStartElementType(int type) const { return 0; }

  // get the element at the given index
  virtual MElement *getMeshElement(unsigned int index) { return 0; }

  // get/set all mesh element visibility flag
  bool getAllElementsVisible(){ return _allElementsVisible ? true : false; }
  void setAllElementsVisible(bool val){ _allElementsVisible = val ? 1 : 0; }

  // get the number of mesh vertices in the entity
  unsigned int getNumMeshVertices() { return mesh_vertices.size(); }

  // get the mesh vertex at the given index
  MVertex *getMeshVertex(unsigned int index) { return mesh_vertices[index]; }

  // add a MeshVertex
  void addMeshVertex(MVertex *v) { mesh_vertices.push_back(v);}

  // relocate mesh vertices using their parametric coordinates
  virtual void relocateMeshVertices(){}

  // clean downcasts
  GVertex *cast2Vertex();
  GEdge   *cast2Edge();
  GFace   *cast2Face();
  GRegion *cast2Region();

  // update all vertex lists, including periodic connections
  void updateVertices(const std::map<MVertex*,MVertex*>&);

  // transformation from master
  std::vector<double> affineTransform;

  // corresponding principal vertices
  std::map<MVertex*,MVertex*> correspondingVertices;

  // corresponding high order control points
  std::map<MVertex*,MVertex*> correspondingHOPoints;

};

class GEntityLessThan {
 public:
  bool operator()(const GEntity *ent1, const GEntity *ent2) const
  {
    return ent1->tag() < ent2->tag();
  }
};

#endif
