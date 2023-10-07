// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _StepRepr_RepresentedDefinition_HeaderFile
#define _StepRepr_RepresentedDefinition_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <StepData_SelectType.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Standard_Transient.hxx>
#include <Handle_StepBasic_GeneralProperty.hxx>
#include <Handle_StepRepr_PropertyDefinition.hxx>
#include <Handle_StepRepr_PropertyDefinitionRelationship.hxx>
#include <Handle_StepRepr_ShapeAspect.hxx>
#include <Handle_StepRepr_ShapeAspectRelationship.hxx>
class Standard_Transient;
class StepBasic_GeneralProperty;
class StepRepr_PropertyDefinition;
class StepRepr_PropertyDefinitionRelationship;
class StepRepr_ShapeAspect;
class StepRepr_ShapeAspectRelationship;


//! Representation of STEP SELECT type RepresentedDefinition
class StepRepr_RepresentedDefinition  : public StepData_SelectType
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Empty constructor
  Standard_EXPORT StepRepr_RepresentedDefinition();
  
  //! Recognizes a kind of RepresentedDefinition select type
  //! 1 -> GeneralProperty from StepBasic
  //! 2 -> PropertyDefinition from StepRepr
  //! 3 -> PropertyDefinitionRelationship from StepRepr
  //! 4 -> ShapeAspect from StepRepr
  //! 5 -> ShapeAspectRelationship from StepRepr
  //! 0 else
  Standard_EXPORT   Standard_Integer CaseNum (const Handle(Standard_Transient)& ent)  const;
  
  //! Returns Value as GeneralProperty (or Null if another type)
  Standard_EXPORT   Handle(StepBasic_GeneralProperty) GeneralProperty()  const;
  
  //! Returns Value as PropertyDefinition (or Null if another type)
  Standard_EXPORT   Handle(StepRepr_PropertyDefinition) PropertyDefinition()  const;
  
  //! Returns Value as PropertyDefinitionRelationship (or Null if another type)
  Standard_EXPORT   Handle(StepRepr_PropertyDefinitionRelationship) PropertyDefinitionRelationship()  const;
  
  //! Returns Value as ShapeAspect (or Null if another type)
  Standard_EXPORT   Handle(StepRepr_ShapeAspect) ShapeAspect()  const;
  
  //! Returns Value as ShapeAspectRelationship (or Null if another type)
  Standard_EXPORT   Handle(StepRepr_ShapeAspectRelationship) ShapeAspectRelationship()  const;




protected:





private:





};







#endif // _StepRepr_RepresentedDefinition_HeaderFile
