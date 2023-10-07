// Created on: 1996-02-13
// Created by: Yves FRICAUD
// Copyright (c) 1996-1999 Matra Datavision
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

//  Modified by skv - Tue Mar 15 16:20:43 2005
// Add methods for supporting history.

#include <BRepOffsetAPI_MakeOffsetShape.ixx>
#include <BRepOffset_MakeOffset.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <Standard_ConstructionError.hxx>

//=======================================================================
//function : BRepOffsetAPI_MakeOffsetShape
//purpose  : 
//=======================================================================

BRepOffsetAPI_MakeOffsetShape::BRepOffsetAPI_MakeOffsetShape()
{
}

//=======================================================================
//function : BRepOffsetAPI_MakeOffsetShape
//purpose  : 
//=======================================================================

BRepOffsetAPI_MakeOffsetShape::BRepOffsetAPI_MakeOffsetShape
(const TopoDS_Shape&    S, 
 const Standard_Real    Offset, 
 const Standard_Real    Tol, 
 const BRepOffset_Mode  Mode, 
 const Standard_Boolean Intersection,
 const Standard_Boolean SelfInter,
 const GeomAbs_JoinType Join)
{
  myOffsetShape.Initialize (S,Offset,Tol,Mode,Intersection,SelfInter,Join);
  Build();
}


//=======================================================================
//function :MakeOffset
//purpose  : 
//=======================================================================

const BRepOffset_MakeOffset& BRepOffsetAPI_MakeOffsetShape::MakeOffset() const 
{
  return myOffsetShape;
}

//=======================================================================
//function : Build
//purpose  : 
//=======================================================================

void BRepOffsetAPI_MakeOffsetShape::Build()
{
  if (!IsDone()) {
    myOffsetShape.MakeOffsetShape();
    if (!myOffsetShape.IsDone()) return;
    myShape  = myOffsetShape.Shape();
    Done();
  }
}


//=======================================================================
//function : Generated
//purpose  : 
//=======================================================================

const TopTools_ListOfShape& BRepOffsetAPI_MakeOffsetShape::Generated (const TopoDS_Shape& S) 

{  
  myGenerated.Clear();
  if (!myOffsetShape.ClosingFaces().Contains(S)) {
    myOffsetShape.OffsetFacesFromShapes ().LastImage (S, myGenerated);
   
    if (!myOffsetShape.ClosingFaces().IsEmpty()) {
      // Reverse generated shapes in case of small solids.
      // Useful only for faces without influence on others.
      TopTools_ListIteratorOfListOfShape it(myGenerated);
      for (; it.More(); it.Next())
	it.Value().Reverse();
    }
  }
  return myGenerated;
}


//  Modified by skv - Tue Mar 15 16:20:43 2005 Begin

//=======================================================================
//function : GeneratedEdge
//purpose  : 
//=======================================================================

const TopTools_ListOfShape& BRepOffsetAPI_MakeOffsetShape::GeneratedEdge (const TopoDS_Shape& S) 

{  
  myGenerated.Clear();
  myOffsetShape.OffsetEdgesFromShapes ().LastImage (S, myGenerated);

  if (!myGenerated.IsEmpty()) {
    if (S.IsSame(myGenerated.First()))
      myGenerated.RemoveFirst();
  }

  return myGenerated;
}


//=======================================================================
//function : GetJoinType
//purpose  : Query offset join type.
//=======================================================================

GeomAbs_JoinType BRepOffsetAPI_MakeOffsetShape::GetJoinType() const
{
  return myOffsetShape.GetJoinType();
}

//  Modified by skv - Tue Mar 15 16:20:43 2005 End
