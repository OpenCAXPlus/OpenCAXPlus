// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopoDS_TFace_HeaderFile
#define _TopoDS_TFace_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TopoDS_TFace.hxx>

#include <TopoDS_TShape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <Handle_TopoDS_TShape.hxx>
class TopoDS_TShape;


//! A  topological part  of a surface   or  of the  2D
//! space.  The  boundary  is  a   set of  wires   and
//! vertices.
class TopoDS_TFace : public TopoDS_TShape
{

public:

  
  //! Creates an empty TFace.
    TopoDS_TFace();
  
  //! returns FACE.
  Standard_EXPORT   TopAbs_ShapeEnum ShapeType()  const;
  
  //! Returns an empty TFace.
  Standard_EXPORT virtual   Handle(TopoDS_TShape) EmptyCopy()  const;




  DEFINE_STANDARD_RTTI(TopoDS_TFace)

protected:




private: 




};


#include <TopoDS_TFace.lxx>





#endif // _TopoDS_TFace_HeaderFile
