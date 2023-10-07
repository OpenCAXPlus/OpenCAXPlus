// Created on: 1997-06-25
// Created by: Philippe MANGIN
// Copyright (c) 1997-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#include <Approx_SweepFunction.ixx>

// Standard_Boolean Approx_SweepFunction::D1(const Standard_Real Param,const Standard_Real First,const Standard_Real Last,TColgp_Array1OfPnt& Poles,TColgp_Array1OfVec& DPoles,TColgp_Array1OfPnt2d& Poles2d,TColgp_Array1OfVec2d& DPoles2d,TColStd_Array1OfReal& Weigths,TColStd_Array1OfReal& DWeigths) 
Standard_Boolean Approx_SweepFunction::D1(const Standard_Real ,const Standard_Real ,const Standard_Real ,TColgp_Array1OfPnt& ,TColgp_Array1OfVec& ,TColgp_Array1OfPnt2d& ,TColgp_Array1OfVec2d& ,TColStd_Array1OfReal& ,TColStd_Array1OfReal& ) 
{
 Standard_NotImplemented::Raise("Approx_SweepFunction::D1");
 return Standard_False;
}

// Standard_Boolean Approx_SweepFunction::D2(const Standard_Real Param,const Standard_Real First,const Standard_Real Last,TColgp_Array1OfPnt& Poles,TColgp_Array1OfVec& DPoles,TColgp_Array1OfVec& D2Poles,TColgp_Array1OfPnt2d& Poles2d,TColgp_Array1OfVec2d& DPoles2d,TColgp_Array1OfVec2d& D2Poles2d,TColStd_Array1OfReal& Weigths,TColStd_Array1OfReal& DWeigths,TColStd_Array1OfReal& D2Weigths) 
 Standard_Boolean Approx_SweepFunction::D2(const Standard_Real ,const Standard_Real ,const Standard_Real ,TColgp_Array1OfPnt& ,TColgp_Array1OfVec& ,TColgp_Array1OfVec& ,TColgp_Array1OfPnt2d& ,TColgp_Array1OfVec2d& ,TColgp_Array1OfVec2d& ,TColStd_Array1OfReal& ,TColStd_Array1OfReal& ,TColStd_Array1OfReal& ) 
{
 Standard_NotImplemented::Raise("Approx_SweepFunction::D2");
 return Standard_False;
}

// void Approx_SweepFunction::Resolution(const Standard_Integer Index,const Standard_Real Tol,Standard_Real& TolU,Standard_Real& TolV) const
 void Approx_SweepFunction::Resolution(const Standard_Integer ,const Standard_Real ,Standard_Real& ,Standard_Real& ) const
{
 Standard_NotImplemented::Raise("Approx_SweepFunction::Resolution");  
}

 gp_Pnt Approx_SweepFunction::BarycentreOfSurf() const
{
 Standard_NotImplemented::Raise("Approx_SweepFunction::BarycentreOfSurf");
 return gp_Pnt(0.,0.,0.);
}

 Standard_Real Approx_SweepFunction::MaximalSection() const
{
  Standard_NotImplemented::Raise("Approx_SweepFunction::MaximalSection()");
  return 0;
}

// void Approx_SweepFunction::GetMinimalWeight(TColStd_Array1OfReal& Weigths) const
 void Approx_SweepFunction::GetMinimalWeight(TColStd_Array1OfReal& ) const
{
 Standard_NotImplemented::Raise("Approx_SweepFunction::GetMinimalWeight");
}
