// Created on: 1997-04-21
// Created by: Prestataire Mary FABIEN
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

#include <TopOpeBRepDS_Filter.ixx>
#include <TopOpeBRepDS_DataStructure.hxx>
#include <TopoDS_Shape.hxx>
#include <TopOpeBRepDS_Interference.hxx>
#include <TopOpeBRepDS_ListOfInterference.hxx>
#include <TopOpeBRepDS_ListIteratorOfListOfInterference.hxx>

#ifdef OCCT_DEBUG
extern Standard_Boolean TopOpeBRepDS_GettracePEI();
extern Standard_Boolean TopOpeBRepDS_GettracePI();
extern Standard_Boolean TopOpeBRepDS_GettraceSPSX(const Standard_Integer);
//unreferenced function, commented
/*static Standard_Boolean TRCE(const Standard_Integer EIX) {
  Standard_Boolean b1 = TopOpeBRepDS_GettracePEI();
  Standard_Boolean b2 = TopOpeBRepDS_GettracePI();
  Standard_Boolean b3 = TopOpeBRepDS_GettraceSPSX(EIX);
  return (b1 || b2 || b3);
}*/
#endif

//=======================================================================
//function : TopOpeBRepDS_Filter
//purpose  : 
//=======================================================================

TopOpeBRepDS_Filter::TopOpeBRepDS_Filter
(const Handle(TopOpeBRepDS_HDataStructure)& HDS,
 const TopOpeBRepTool_PShapeClassifier& pClassif)
: myHDS(HDS),
  myPShapeClassif(pClassif)
{}


//=======================================================================
//function : ProcessInterferences
//purpose  : 
//=======================================================================

void TopOpeBRepDS_Filter::ProcessInterferences()
{
  ProcessEdgeInterferences();
  ProcessCurveInterferences();
}

//=======================================================================
//function : ProcessEdgeInterferences
//purpose  : 
//=======================================================================

void TopOpeBRepDS_Filter::ProcessEdgeInterferences()
{
  TopOpeBRepDS_DataStructure& BDS = myHDS->ChangeDS();
  Standard_Integer i,nshape = BDS.NbShapes();

  for (i = 1; i <= nshape; i++) {
    const TopoDS_Shape& S = BDS.Shape(i);
    if(S.IsNull()) continue;
    if ( S.ShapeType() == TopAbs_EDGE ) {
      ProcessEdgeInterferences(i);
    }
  }
}

//=======================================================================
//function : ProcessFaceInterferences
//purpose  : 
//=======================================================================

void TopOpeBRepDS_Filter::ProcessFaceInterferences
(const TopOpeBRepDS_DataMapOfShapeListOfShapeOn1State& MEsp)
{
  TopOpeBRepDS_DataStructure& BDS = myHDS->ChangeDS();
  Standard_Integer i,nshape = BDS.NbShapes();

  for (i = 1; i <= nshape; i++) {
    const TopoDS_Shape& S = BDS.Shape(i);
    if(S.IsNull()) continue;
    if ( S.ShapeType() == TopAbs_FACE ) {
      ProcessFaceInterferences(i,MEsp);
    }
  }
}

//=======================================================================
//function : ProcessCurveInterferences
//purpose  : 
//=======================================================================

void TopOpeBRepDS_Filter::ProcessCurveInterferences()
{
  TopOpeBRepDS_DataStructure& BDS = myHDS->ChangeDS();
  Standard_Integer i,ncurve = BDS.NbCurves();
  for (i = 1; i <= ncurve; i++) {
    ProcessCurveInterferences(i);
  }
}

// ProcessFaceInterferences  : voir FilterFaceInterferences.cxx
// ProcessEdgeInterferences  : voir FilterEdgeInterferences.cxx
// ProcessCurveInterferences : voir FilterCurveInterferences.cxx
