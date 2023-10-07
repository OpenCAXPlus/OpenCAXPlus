// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Draft_DataMapIteratorOfDataMapOfFaceFaceInfo_HeaderFile
#define _Draft_DataMapIteratorOfDataMapOfFaceFaceInfo_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMapIterator.hxx>
#include <Handle_Draft_DataMapNodeOfDataMapOfFaceFaceInfo.hxx>
class Standard_NoSuchObject;
class TopoDS_Face;
class Draft_FaceInfo;
class TopTools_ShapeMapHasher;
class Draft_DataMapOfFaceFaceInfo;
class Draft_DataMapNodeOfDataMapOfFaceFaceInfo;



class Draft_DataMapIteratorOfDataMapOfFaceFaceInfo  : public TCollection_BasicMapIterator
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT Draft_DataMapIteratorOfDataMapOfFaceFaceInfo();
  
  Standard_EXPORT Draft_DataMapIteratorOfDataMapOfFaceFaceInfo(const Draft_DataMapOfFaceFaceInfo& aMap);
  
  Standard_EXPORT   void Initialize (const Draft_DataMapOfFaceFaceInfo& aMap) ;
  
  Standard_EXPORT  const  TopoDS_Face& Key()  const;
  
  Standard_EXPORT  const  Draft_FaceInfo& Value()  const;




protected:





private:





};







#endif // _Draft_DataMapIteratorOfDataMapOfFaceFaceInfo_HeaderFile
