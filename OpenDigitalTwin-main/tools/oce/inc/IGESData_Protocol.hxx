// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _IGESData_Protocol_HeaderFile
#define _IGESData_Protocol_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_IGESData_Protocol.hxx>

#include <Interface_Protocol.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Interface_Protocol.hxx>
#include <Handle_Standard_Type.hxx>
#include <Handle_Interface_InterfaceModel.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_Standard_Transient.hxx>
class Interface_Protocol;
class Standard_Type;
class Interface_InterfaceModel;
class Standard_Transient;


//! Description of basic Protocol for IGES
//! This comprises treatement of IGESModel and Recognition of
//! Undefined-FreeFormat-Entity
class IGESData_Protocol : public Interface_Protocol
{

public:

  
  Standard_EXPORT IGESData_Protocol();
  
  //! Gives the count of Resource Protocol. Here, none
  Standard_EXPORT   Standard_Integer NbResources()  const;
  
  //! Returns a Resource, given a rank. Here, none
  Standard_EXPORT   Handle(Interface_Protocol) Resource (const Standard_Integer num)  const;
  
  //! Returns a Case Number, specific of each recognized Type
  //! Here, Undefined and Free Format Entities have the Number 1.
  Standard_EXPORT   Standard_Integer TypeNumber (const Handle(Standard_Type)& atype)  const;
  
  //! Creates an empty Model for IGES Norm
  Standard_EXPORT   Handle(Interface_InterfaceModel) NewModel()  const;
  
  //! Returns True if <model> is a Model of IGES Norm
  Standard_EXPORT   Standard_Boolean IsSuitableModel (const Handle(Interface_InterfaceModel)& model)  const;
  
  //! Creates a new Unknown Entity for IGES (UndefinedEntity)
  Standard_EXPORT   Handle(Standard_Transient) UnknownEntity()  const;
  
  //! Returns True if <ent> is an Unknown Entity for the Norm, i.e.
  //! Type UndefinedEntity, status Unknown
  Standard_EXPORT   Standard_Boolean IsUnknownEntity (const Handle(Standard_Transient)& ent)  const;




  DEFINE_STANDARD_RTTI(IGESData_Protocol)

protected:




private: 




};







#endif // _IGESData_Protocol_HeaderFile
