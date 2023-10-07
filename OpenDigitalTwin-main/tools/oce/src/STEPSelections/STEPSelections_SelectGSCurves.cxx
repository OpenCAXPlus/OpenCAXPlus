// Created on: 1999-03-22
// Created by: data exchange team
// Copyright (c) 1999-1999 Matra Datavision
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

#include <STEPSelections_SelectGSCurves.ixx>
#include <StepGeom_Curve.hxx>
#include <StepGeom_CompositeCurve.hxx>
#include <StepShape_GeometricSet.hxx>
#include <StepGeom_CompositeCurveSegment.hxx>

static Standard_Integer flag;

STEPSelections_SelectGSCurves::STEPSelections_SelectGSCurves():IFSelect_SelectExplore (-1){ flag = 1;}
     
Standard_Boolean STEPSelections_SelectGSCurves::Explore(const Standard_Integer /*level*/,
						       const Handle(Standard_Transient)& start,
						       const Interface_Graph& G,
						       Interface_EntityIterator& explored) const
{
  if(start.IsNull()) return Standard_False;
  
  if (start->IsKind(STANDARD_TYPE(StepGeom_Curve))) {
    if(start->IsKind(STANDARD_TYPE(StepGeom_CompositeCurve))) {
      Interface_EntityIterator subs = G.Sharings(start);
      Standard_Boolean isInGeomSet = Standard_False;
      for (subs.Start(); subs.More()&&!isInGeomSet; subs.Next()) 
	if(subs.Value()->IsKind(STANDARD_TYPE(StepShape_GeometricSet))){
	  if(flag) {
	    explored.AddItem (subs.Value());
	    flag =0;
	  }
	  isInGeomSet = Standard_True; 
	}
      if(isInGeomSet) {
	Interface_EntityIterator subs = G.Shareds(start);
	subs.Start();
	Standard_Boolean isSome = subs.More();
	for (; subs.More(); subs.Next()) 
	  explored.AddItem (subs.Value());
	return isSome;
      } else
	return Standard_False;
    } else {
      Interface_EntityIterator subs = G.Sharings(start);
      for (subs.Start(); subs.More(); subs.Next()) {
	if(subs.Value()->IsKind(STANDARD_TYPE(StepShape_GeometricSet))||
	   subs.Value()->IsKind(STANDARD_TYPE(StepGeom_CompositeCurveSegment)))
	  return Standard_True;
      }
    }
  }
  
  Interface_EntityIterator subs = G.Shareds(start);
  subs.Start();
  Standard_Boolean isSome = subs.More();
  for (; subs.More(); subs.Next()) 
    explored.AddItem (subs.Value());
  
  return isSome;
}

TCollection_AsciiString STEPSelections_SelectGSCurves::ExploreLabel() const
{
  return TCollection_AsciiString ("Curves in GS");
}
