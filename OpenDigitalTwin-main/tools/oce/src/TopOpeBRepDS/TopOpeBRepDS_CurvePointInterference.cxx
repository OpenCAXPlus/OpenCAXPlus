// Created on: 1993-06-23
// Created by: Jean Yves LEBEY
// Copyright (c) 1993-1999 Matra Datavision
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

#include <TopOpeBRepDS_CurvePointInterference.ixx>

//=======================================================================
//function : TopOpeBRepDS_CurvePointInterference
//purpose  : 
//=======================================================================

TopOpeBRepDS_CurvePointInterference::TopOpeBRepDS_CurvePointInterference
  (const TopOpeBRepDS_Transition& T, 
   const TopOpeBRepDS_Kind ST,
   const Standard_Integer S, 
   const TopOpeBRepDS_Kind GT,
   const Standard_Integer G, 
   const Standard_Real P) :
  TopOpeBRepDS_Interference(T,ST,S,GT,G),
  myParam(P)
{
}


//=======================================================================
//function : Parameter
//purpose  : 
//=======================================================================

Standard_Real  TopOpeBRepDS_CurvePointInterference::Parameter()const 
{
  return myParam;
}


//=======================================================================
//function : Parameter
//purpose  : 
//=======================================================================

void  TopOpeBRepDS_CurvePointInterference::Parameter(const Standard_Real P)
{
  myParam = P;
}


//=======================================================================
//function : Dump
//purpose  : 
//=======================================================================

Standard_OStream& TopOpeBRepDS_CurvePointInterference::Dump
  (Standard_OStream& OS) const
{
#ifdef OCCT_DEBUG
  TopOpeBRepDS_Kind supporttype = SupportType();
  if      (supporttype == TopOpeBRepDS_EDGE)  OS<<"EPI";
  else if (supporttype == TopOpeBRepDS_CURVE) OS<<"CPI";
  else if (supporttype == TopOpeBRepDS_FACE)  OS<<"ECPI";
  else                                        OS<<"???";
  OS<<" "; TopOpeBRepDS_Interference::Dump(OS);
  OS<<" "<<myParam; 
#endif

  return OS;
}
