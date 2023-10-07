// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _MAT2d_DataMapOfIntegerBisec_HeaderFile
#define _MAT2d_DataMapOfIntegerBisec_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMap.hxx>
#include <Standard_Integer.hxx>
#include <Handle_MAT2d_DataMapNodeOfDataMapOfIntegerBisec.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Address.hxx>
class Standard_DomainError;
class Standard_NoSuchObject;
class Bisector_Bisec;
class TColStd_MapIntegerHasher;
class MAT2d_DataMapNodeOfDataMapOfIntegerBisec;
class MAT2d_DataMapIteratorOfDataMapOfIntegerBisec;



class MAT2d_DataMapOfIntegerBisec  : public TCollection_BasicMap
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT MAT2d_DataMapOfIntegerBisec(const Standard_Integer NbBuckets = 1);
  
  Standard_EXPORT   MAT2d_DataMapOfIntegerBisec& Assign (const MAT2d_DataMapOfIntegerBisec& Other) ;
  MAT2d_DataMapOfIntegerBisec& operator = (const MAT2d_DataMapOfIntegerBisec& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT   void ReSize (const Standard_Integer NbBuckets) ;
  
  Standard_EXPORT   void Clear() ;
~MAT2d_DataMapOfIntegerBisec()
{
  Clear();
}
  
  Standard_EXPORT   Standard_Boolean Bind (const Standard_Integer& K, const Bisector_Bisec& I) ;
  
  Standard_EXPORT   Standard_Boolean IsBound (const Standard_Integer& K)  const;
  
  Standard_EXPORT   Standard_Boolean UnBind (const Standard_Integer& K) ;
  
  Standard_EXPORT  const  Bisector_Bisec& Find (const Standard_Integer& K)  const;
 const  Bisector_Bisec& operator() (const Standard_Integer& K)  const
{
  return Find(K);
}
  
  Standard_EXPORT   Bisector_Bisec& ChangeFind (const Standard_Integer& K) ;
  Bisector_Bisec& operator() (const Standard_Integer& K) 
{
  return ChangeFind(K);
}
  
  Standard_EXPORT   Standard_Address Find1 (const Standard_Integer& K)  const;
  
  Standard_EXPORT   Standard_Address ChangeFind1 (const Standard_Integer& K) ;




protected:





private:

  
  Standard_EXPORT MAT2d_DataMapOfIntegerBisec(const MAT2d_DataMapOfIntegerBisec& Other);




};







#endif // _MAT2d_DataMapOfIntegerBisec_HeaderFile
