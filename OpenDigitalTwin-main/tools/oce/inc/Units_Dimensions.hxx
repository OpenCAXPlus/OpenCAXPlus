// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Units_Dimensions_HeaderFile
#define _Units_Dimensions_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Units_Dimensions.hxx>

#include <Standard_Real.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_CString.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>


//! This class includes all  the methods to create and
//! manipulate    the   dimensions  of the    physical
//! quantities.
class Units_Dimensions : public MMgt_TShared
{

public:

  
  //! Returns  a  Dimensions  object  which  represents  the
  //! dimension  of  a  physical  quantity.    Each  of  the
  //! <amass>,  <alength>,   <atime>,   <anelectriccurrent>,
  //! <athermodynamictemperature>,    <anamountofsubstance>,
  //! <aluminousintensity>, <aplaneangle>, <asolidangle> are
  //! the powers for  the 7  fundamental  units of  physical
  //! quantity and  the 2  secondary  fundamental  units  of
  //! physical quantity.
  Standard_EXPORT Units_Dimensions(const Standard_Real amass, const Standard_Real alength, const Standard_Real atime, const Standard_Real anelectriccurrent, const Standard_Real athermodynamictemperature, const Standard_Real anamountofsubstance, const Standard_Real aluminousintensity, const Standard_Real aplaneangle, const Standard_Real asolidangle);
  
  //! Returns the power of mass stored in the dimensions.
      Standard_Real Mass()  const;
  
  //! Returns the power of length stored in the dimensions.
      Standard_Real Length()  const;
  
  //! Returns the power of time stored in the dimensions.
      Standard_Real Time()  const;
  
  //! Returns the  power of  electrical  intensity (current)
  //! stored in the dimensions.
      Standard_Real ElectricCurrent()  const;
  
  //! Returns  the  power  of   temperature stored  in   the
  //! dimensions.
      Standard_Real ThermodynamicTemperature()  const;
  
  //! Returns  the power   of quantity   of  material (mole)
  //! stored in the dimensions.
      Standard_Real AmountOfSubstance()  const;
  
  //! Returns the  power of light   intensity stored  in the
  //! dimensions.
      Standard_Real LuminousIntensity()  const;
  
  //! Returns  the power   of plane   angle  stored  in  the
  //! dimensions.
      Standard_Real PlaneAngle()  const;
  
  //! Returns the   power   of  solid angle stored   in  the
  //! dimensions.
      Standard_Real SolidAngle()  const;
  
  //! Returns the quantity string of the dimension
  Standard_EXPORT   Standard_CString Quantity()  const;
  
  //! Creates and returns  a new Dimensions  object which is
  //! the   result   of the  multiplication    of  <me>  and
  //! <adimensions>.
  Standard_EXPORT   Handle(Units_Dimensions) Multiply (const Handle(Units_Dimensions)& adimensions)  const;
  
  //! Creates and returns a new  Dimensions object which  is
  //! the result of the division of <me> by <adimensions>.
  Standard_EXPORT   Handle(Units_Dimensions) Divide (const Handle(Units_Dimensions)& adimensions)  const;
  
  //! Creates  and returns a new  Dimensions object which is
  //! the result of the power of <me> and <anexponent>.
  Standard_EXPORT   Handle(Units_Dimensions) Power (const Standard_Real anexponent)  const;
  
  //! Returns true if  <me>  and <adimensions> have the same
  //! dimensions, false otherwise.
  Standard_EXPORT   Standard_Boolean IsEqual (const Handle(Units_Dimensions)& adimensions)  const;
  
  //! Returns false if <me> and  <adimensions> have the same
  //! dimensions, true otherwise.
  Standard_EXPORT   Standard_Boolean IsNotEqual (const Handle(Units_Dimensions)& adimensions)  const;
  
  //! Useful for degugging.
  Standard_EXPORT   void Dump (const Standard_Integer ashift)  const;
  
  Standard_EXPORT static   Handle(Units_Dimensions) ALess() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) AMass() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) ALength() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) ATime() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) AElectricCurrent() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) AThermodynamicTemperature() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) AAmountOfSubstance() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) ALuminousIntensity() ;
  
  Standard_EXPORT static   Handle(Units_Dimensions) APlaneAngle() ;
  
  //! Returns the basic dimensions.
  Standard_EXPORT static   Handle(Units_Dimensions) ASolidAngle() ;




  DEFINE_STANDARD_RTTI(Units_Dimensions)

protected:




private: 


  Standard_Real themass;
  Standard_Real thelength;
  Standard_Real thetime;
  Standard_Real theelectriccurrent;
  Standard_Real thethermodynamictemperature;
  Standard_Real theamountofsubstance;
  Standard_Real theluminousintensity;
  Standard_Real theplaneangle;
  Standard_Real thesolidangle;


};


#include <Units_Dimensions.lxx>





#endif // _Units_Dimensions_HeaderFile
