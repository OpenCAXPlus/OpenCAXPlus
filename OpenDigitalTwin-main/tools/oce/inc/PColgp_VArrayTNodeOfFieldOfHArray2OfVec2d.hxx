// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d_HeaderFile
#define _PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <gp_Vec2d.hxx>
#include <Handle_PColgp_VArrayNodeOfFieldOfHArray2OfVec2d.hxx>
#include <Standard_Address.hxx>
class gp_Vec2d;
class PColgp_FieldOfHArray2OfVec2d;
class PColgp_VArrayNodeOfFieldOfHArray2OfVec2d;



class PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d 
{
public:

  DEFINE_STANDARD_ALLOC

  
    PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d();
  
    PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d(const gp_Vec2d& aValue);
  
      void SetValue (const gp_Vec2d& aValue) ;
  
      Standard_Address Value()  const;




protected:





private:



  gp_Vec2d myValue;


};

#define Item gp_Vec2d
#define Item_hxx <gp_Vec2d.hxx>
#define DBC_VArrayNode PColgp_VArrayNodeOfFieldOfHArray2OfVec2d
#define DBC_VArrayNode_hxx <PColgp_VArrayNodeOfFieldOfHArray2OfVec2d.hxx>
#define DBC_VArrayTNode PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d
#define DBC_VArrayTNode_hxx <PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d.hxx>
#define Handle_DBC_VArrayNode Handle_PColgp_VArrayNodeOfFieldOfHArray2OfVec2d
#define DBC_VArrayNode_Type_() PColgp_VArrayNodeOfFieldOfHArray2OfVec2d_Type_()
#define DBC_VArray PColgp_FieldOfHArray2OfVec2d
#define DBC_VArray_hxx <PColgp_FieldOfHArray2OfVec2d.hxx>

#include <DBC_VArrayTNode.lxx>

#undef Item
#undef Item_hxx
#undef DBC_VArrayNode
#undef DBC_VArrayNode_hxx
#undef DBC_VArrayTNode
#undef DBC_VArrayTNode_hxx
#undef Handle_DBC_VArrayNode
#undef DBC_VArrayNode_Type_
#undef DBC_VArray
#undef DBC_VArray_hxx




#endif // _PColgp_VArrayTNodeOfFieldOfHArray2OfVec2d_HeaderFile
