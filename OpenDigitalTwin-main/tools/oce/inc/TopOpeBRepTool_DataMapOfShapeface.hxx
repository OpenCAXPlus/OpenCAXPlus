// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepTool_DataMapOfShapeface_HeaderFile
#define _TopOpeBRepTool_DataMapOfShapeface_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMap.hxx>
#include <Handle_TopOpeBRepTool_DataMapNodeOfDataMapOfShapeface.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Address.hxx>
class Standard_DomainError;
class Standard_NoSuchObject;
class TopoDS_Shape;
class TopOpeBRepTool_face;
class TopTools_ShapeMapHasher;
class TopOpeBRepTool_DataMapNodeOfDataMapOfShapeface;
class TopOpeBRepTool_DataMapIteratorOfDataMapOfShapeface;



class TopOpeBRepTool_DataMapOfShapeface  : public TCollection_BasicMap
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TopOpeBRepTool_DataMapOfShapeface(const Standard_Integer NbBuckets = 1);
  
  Standard_EXPORT   TopOpeBRepTool_DataMapOfShapeface& Assign (const TopOpeBRepTool_DataMapOfShapeface& Other) ;
  TopOpeBRepTool_DataMapOfShapeface& operator = (const TopOpeBRepTool_DataMapOfShapeface& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT   void ReSize (const Standard_Integer NbBuckets) ;
  
  Standard_EXPORT   void Clear() ;
~TopOpeBRepTool_DataMapOfShapeface()
{
  Clear();
}
  
  Standard_EXPORT   Standard_Boolean Bind (const TopoDS_Shape& K, const TopOpeBRepTool_face& I) ;
  
  Standard_EXPORT   Standard_Boolean IsBound (const TopoDS_Shape& K)  const;
  
  Standard_EXPORT   Standard_Boolean UnBind (const TopoDS_Shape& K) ;
  
  Standard_EXPORT  const  TopOpeBRepTool_face& Find (const TopoDS_Shape& K)  const;
 const  TopOpeBRepTool_face& operator() (const TopoDS_Shape& K)  const
{
  return Find(K);
}
  
  Standard_EXPORT   TopOpeBRepTool_face& ChangeFind (const TopoDS_Shape& K) ;
  TopOpeBRepTool_face& operator() (const TopoDS_Shape& K) 
{
  return ChangeFind(K);
}
  
  Standard_EXPORT   Standard_Address Find1 (const TopoDS_Shape& K)  const;
  
  Standard_EXPORT   Standard_Address ChangeFind1 (const TopoDS_Shape& K) ;




protected:





private:

  
  Standard_EXPORT TopOpeBRepTool_DataMapOfShapeface(const TopOpeBRepTool_DataMapOfShapeface& Other);




};







#endif // _TopOpeBRepTool_DataMapOfShapeface_HeaderFile
