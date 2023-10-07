// Created on: 2004-05-17
// Created by: Sergey ZARITCHNY
// Copyright (c) 2004-2014 OPEN CASCADE SAS
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

#include <BinMPrsStd.ixx>
#include <CDM_MessageDriver.hxx>
#include <BinMDF_ADriverTable.hxx>
#include <BinMPrsStd_AISPresentationDriver.hxx>
#include <BinMPrsStd_PositionDriver.hxx>

//=======================================================================
//function : AddDrivers
//purpose  : 
//=======================================================================

void BinMPrsStd::AddDrivers (const Handle(BinMDF_ADriverTable)& aDriverTable,
				    const Handle(CDM_MessageDriver)&   aMessageDriver)
{
  aDriverTable->AddDriver (new BinMPrsStd_AISPresentationDriver(aMessageDriver));
  aDriverTable->AddDriver (new BinMPrsStd_PositionDriver(aMessageDriver));
}
