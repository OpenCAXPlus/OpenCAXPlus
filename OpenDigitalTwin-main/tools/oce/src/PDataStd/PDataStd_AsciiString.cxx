// Created on: 2007-08-22
// Created by: Sergey ZARITCHNY
// Copyright (c) 2007-2014 OPEN CASCADE SAS
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

#include <PDataStd_AsciiString.ixx>
#include <PCollection_HAsciiString.hxx>
//=======================================================================
//function : PDataStd_AsciiString
//purpose  : Constructor
//=======================================================================
PDataStd_AsciiString::PDataStd_AsciiString() {}

//=======================================================================
//function : PDataStd_AsciiString
//purpose  : Constructor
//=======================================================================
PDataStd_AsciiString::PDataStd_AsciiString(const Handle(PCollection_HAsciiString)& V)
: myValue (V) {}

//=======================================================================
//function : Get
//purpose  : 
//=======================================================================
Handle(PCollection_HAsciiString) PDataStd_AsciiString::Get() const
{ return myValue; }

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
void PDataStd_AsciiString::Set(const Handle(PCollection_HAsciiString)& V) 
{ myValue = V; }

