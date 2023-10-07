// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepShape_DimensionalLocationWithPath_HeaderFile
#define _StepShape_DimensionalLocationWithPath_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_StepShape_DimensionalLocationWithPath.hxx>

#include <Handle_StepRepr_ShapeAspect.hxx>
#include <StepShape_DimensionalLocation.hxx>
#include <Handle_TCollection_HAsciiString.hxx>
#include <Standard_Boolean.hxx>
class StepRepr_ShapeAspect;
class TCollection_HAsciiString;


//! Representation of STEP entity DimensionalLocationWithPath
class StepShape_DimensionalLocationWithPath : public StepShape_DimensionalLocation
{

public:

  
  //! Empty constructor
  Standard_EXPORT StepShape_DimensionalLocationWithPath();
  
  //! Initialize all fields (own and inherited)
  Standard_EXPORT   void Init (const Handle(TCollection_HAsciiString)& aShapeAspectRelationship_Name, const Standard_Boolean hasShapeAspectRelationship_Description, const Handle(TCollection_HAsciiString)& aShapeAspectRelationship_Description, const Handle(StepRepr_ShapeAspect)& aShapeAspectRelationship_RelatingShapeAspect, const Handle(StepRepr_ShapeAspect)& aShapeAspectRelationship_RelatedShapeAspect, const Handle(StepRepr_ShapeAspect)& aPath) ;
  
  //! Returns field Path
  Standard_EXPORT   Handle(StepRepr_ShapeAspect) Path()  const;
  
  //! Set field Path
  Standard_EXPORT   void SetPath (const Handle(StepRepr_ShapeAspect)& Path) ;




  DEFINE_STANDARD_RTTI(StepShape_DimensionalLocationWithPath)

protected:




private: 


  Handle(StepRepr_ShapeAspect) thePath;


};







#endif // _StepShape_DimensionalLocationWithPath_HeaderFile
