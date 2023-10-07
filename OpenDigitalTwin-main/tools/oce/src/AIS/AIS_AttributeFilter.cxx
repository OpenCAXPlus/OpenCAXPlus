// Created on: 1997-03-04
// Created by: Robert COUBLANC
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

#include <AIS_AttributeFilter.ixx>

#include <AIS_InteractiveObject.hxx>


AIS_AttributeFilter::AIS_AttributeFilter():
hasC(Standard_False),
hasW(Standard_False){}


AIS_AttributeFilter::AIS_AttributeFilter(const Quantity_NameOfColor aCol):
myCol(aCol),
hasC(Standard_True),
hasW(Standard_False){}


AIS_AttributeFilter::AIS_AttributeFilter(const Standard_Real aWid):
myWid(aWid),
hasC(Standard_False),
hasW(Standard_True){}


Standard_Boolean AIS_AttributeFilter::IsOk(const Handle(SelectMgr_EntityOwner)& anObj) const 
{
  if (Handle(AIS_InteractiveObject)::DownCast(anObj->Selectable()).IsNull()) return Standard_False;
  Standard_Boolean okstat = Standard_True;

//#ifndef OCCT_DEBUG
  Handle(SelectMgr_SelectableObject) aSelectable = anObj->Selectable() ;
  if( hasC && ((Handle(AIS_InteractiveObject)&) aSelectable)->HasColor() )
//#else
//  if(hasC && ((Handle(AIS_InteractiveObject)&) anObj->Selectable())->HasColor())
//#endif
    okstat =  (myCol == ((Handle(AIS_InteractiveObject)&) anObj)->Color());

//#ifndef OCCT_DEBUG
  aSelectable = anObj->Selectable() ;
  if( hasW && ((Handle(AIS_InteractiveObject)&) aSelectable)->HasWidth() )
//#else
//  if(hasW && ((Handle(AIS_InteractiveObject)&) anObj->Selectable())->HasWidth())
//#endif
    okstat =  (myWid == ((Handle(AIS_InteractiveObject)&) anObj)->Width()) && okstat;

  return okstat;
  
}
