// Created on: 1997-04-09
// Created by: VAUTHIER Jean-Claude
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

#include <PDataStd_Real.ixx>


//=======================================================================
//function : PDataStd_Real
//purpose  : 
//=======================================================================

PDataStd_Real::PDataStd_Real () : myValue(0.),myDimension(0) { }


//=======================================================================
//function : PDataStd_Real
//purpose  : 
//=======================================================================

PDataStd_Real::PDataStd_Real (const Standard_Real V, 
			      const Standard_Integer D) : myValue (V), myDimension(D) { }


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

void PDataStd_Real::Set (const Standard_Real V) 
{
  myValue = V;
}



//=======================================================================
//function : Get
//purpose  : 
//=======================================================================

Standard_Real PDataStd_Real::Get () const
{
  return myValue;
}

//=======================================================================
//function : SetDimension
//purpose  : 
//=======================================================================

void PDataStd_Real::SetDimension (const Standard_Integer D)
{
  myDimension = D;
}



//=======================================================================
//function : GetDimension
//purpose  : 
//=======================================================================

Standard_Integer PDataStd_Real::GetDimension () const
{
  return myDimension;
}


