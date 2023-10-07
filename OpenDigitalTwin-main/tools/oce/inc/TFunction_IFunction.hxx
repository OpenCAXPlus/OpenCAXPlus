// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TFunction_IFunction_HeaderFile
#define _TFunction_IFunction_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TDF_Label.hxx>
#include <Standard_Boolean.hxx>
#include <TFunction_ExecutionStatus.hxx>
#include <Handle_TFunction_Driver.hxx>
#include <Standard_Integer.hxx>
#include <Handle_TFunction_GraphNode.hxx>
class TDF_Label;
class Standard_GUID;
class TDF_LabelList;
class TFunction_DoubleMapOfIntegerLabel;
class TFunction_Logbook;
class TFunction_Driver;
class TFunction_GraphNode;


//! Interface class for usage of Function Mechanism
class TFunction_IFunction 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Sets a new function attached to a label <L> with <ID>.
  //! It creates a new TFunction_Function attribute initialized by the <ID>,
  //! a new TFunction_GraphNode with an empty list of dependencies and
  //! the status equal to TFunction_ES_WrongDefinition.
  //! It registers the function in the scope of functions for this document.
  Standard_EXPORT static   Standard_Boolean NewFunction (const TDF_Label& L, const Standard_GUID& ID) ;
  
  //! Deletes a function attached to a label <L>.
  //! It deletes a TFunction_Function attribute and a TFunction_GraphNode.
  //! It deletes the functions from the scope of function of this document.
  Standard_EXPORT static   Standard_Boolean DeleteFunction (const TDF_Label& L) ;
  
  //! Updates dependencies for all functions of the scope.
  //! It returns false in case of an error.
  //! An empty constructor.
  Standard_EXPORT static   Standard_Boolean UpdateDependencies (const TDF_Label& Access) ;
  
  Standard_EXPORT TFunction_IFunction();
  
  //! A constructor.
  //! Initializes the interface by the label of function.
  Standard_EXPORT TFunction_IFunction(const TDF_Label& L);
  
  //! Initializes the interface by the label of function.
  Standard_EXPORT   void Init (const TDF_Label& L) ;
  
  //! Returns a label of the function.
  Standard_EXPORT  const  TDF_Label& Label()  const;
  
  //! Updates the dependencies of this function only.
  Standard_EXPORT   Standard_Boolean UpdateDependencies()  const;
  
  //! The method fills-in the list by labels,
  //! where the arguments of the function are located.
  Standard_EXPORT   void Arguments (TDF_LabelList& args)  const;
  
  //! The method fills-in the list by labels,
  //! where the results of the function are located.
  Standard_EXPORT   void Results (TDF_LabelList& res)  const;
  
  //! Returns a list of previous functions.
  Standard_EXPORT   void GetPrevious (TDF_LabelList& prev)  const;
  
  //! Returns a list of next functions.
  Standard_EXPORT   void GetNext (TDF_LabelList& prev)  const;
  
  //! Returns the execution status of the function.
  Standard_EXPORT   TFunction_ExecutionStatus GetStatus()  const;
  
  //! Defines an execution status for a function.
  Standard_EXPORT   void SetStatus (const TFunction_ExecutionStatus status)  const;
  
  //! Returns the scope of all functions.
  Standard_EXPORT  const  TFunction_DoubleMapOfIntegerLabel& GetAllFunctions()  const;
  
  //! Returns the Logbook - keeper of modifications.
  Standard_EXPORT   TFunction_Logbook& GetLogbook()  const;
  
  //! Returns a driver of the function.
  Standard_EXPORT   Handle(TFunction_Driver) GetDriver (const Standard_Integer thread = 0)  const;
  
  //! Returns a graph node of the function.
  Standard_EXPORT   Handle(TFunction_GraphNode) GetGraphNode()  const;




protected:





private:



  TDF_Label myLabel;


};







#endif // _TFunction_IFunction_HeaderFile
