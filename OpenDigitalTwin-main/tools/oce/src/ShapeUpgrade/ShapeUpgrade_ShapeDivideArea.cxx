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

#include <ShapeUpgrade_ShapeDivideArea.ixx>
#include <ShapeUpgrade_FaceDivideArea.hxx>
#include <Precision.hxx>

//=======================================================================
//function : ShapeUpgrade_ShapeDivideArea
//purpose  : 
//=======================================================================

ShapeUpgrade_ShapeDivideArea::ShapeUpgrade_ShapeDivideArea():
       ShapeUpgrade_ShapeDivide()
{
  myMaxArea = Precision::Infinite();
}

//=======================================================================
//function : ShapeUpgrade_ShapeDivideArea
//purpose  : 
//=======================================================================

ShapeUpgrade_ShapeDivideArea::ShapeUpgrade_ShapeDivideArea(const TopoDS_Shape& S):
       ShapeUpgrade_ShapeDivide(S)
       
{
  myMaxArea = Precision::Infinite();
}

//=======================================================================
//function : GetSplitFaceTool
//purpose  : 
//=======================================================================

 Handle(ShapeUpgrade_FaceDivide) ShapeUpgrade_ShapeDivideArea::GetSplitFaceTool() const
{
  Handle(ShapeUpgrade_FaceDivideArea) aFaceTool = new ShapeUpgrade_FaceDivideArea;
  aFaceTool->MaxArea() = myMaxArea;
  return aFaceTool;
}

