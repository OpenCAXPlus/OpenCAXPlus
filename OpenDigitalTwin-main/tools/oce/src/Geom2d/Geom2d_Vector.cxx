// Created on: 1993-03-24
// Created by: JCV
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

#include <Geom2d_Vector.ixx>

typedef Geom2d_Vector         Vector;
typedef Handle(Geom2d_Vector) Handle(Vector);
typedef gp_Ax2d  Ax2d;
typedef gp_Pnt2d Pnt2d;



Standard_Real Geom2d_Vector::Angle (
const Handle(Geom2d_Vector)& Other) const { 

  return gpVec2d.Angle (Other->Vec2d());
}


void Geom2d_Vector::Reverse () { gpVec2d.Reverse(); }

gp_Vec2d Geom2d_Vector::Vec2d () const      { return gpVec2d; }

Standard_Real Geom2d_Vector::X () const              { return gpVec2d.X(); }

Standard_Real Geom2d_Vector::Y () const              { return gpVec2d.Y(); }


Handle(Vector) Geom2d_Vector::Reversed () const {

  Handle(Vector) V = Handle(Vector)::DownCast(Copy());
  V->Reverse();
  return V;
}


void Geom2d_Vector::Coord (Standard_Real& X, Standard_Real& Y) const {

  gpVec2d.Coord (X, Y);
}
 

Standard_Real Geom2d_Vector::Dot (const Handle(Vector)& Other) const  { 

  return gpVec2d.Dot (Other->Vec2d());
}
