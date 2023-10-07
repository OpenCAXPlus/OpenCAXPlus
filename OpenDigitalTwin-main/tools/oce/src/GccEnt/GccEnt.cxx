// Copyright (c) 1995-1999 Matra Datavision
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

//=========================================================================
//   Methods of packing allowing to qualify  objects.              +
//                                                                        +
//=========================================================================

#include <GccEnt.ixx>

GccEnt_QualifiedLin
  GccEnt::Unqualified(const gp_Lin2d& Line) {
    return GccEnt_QualifiedLin(Line,GccEnt_unqualified);
  }

GccEnt_QualifiedCirc
  GccEnt::Unqualified(const gp_Circ2d& Circle) {
    return GccEnt_QualifiedCirc(Circle,GccEnt_unqualified);
  }

GccEnt_QualifiedCirc
  GccEnt::Enclosing(const gp_Circ2d& Circle) {
    return GccEnt_QualifiedCirc(Circle,GccEnt_enclosing);
  }

GccEnt_QualifiedLin
  GccEnt::Enclosed(const gp_Lin2d& Line) {
    return GccEnt_QualifiedLin(Line,GccEnt_enclosed);
  }

GccEnt_QualifiedCirc
  GccEnt::Enclosed(const gp_Circ2d& Circle) {
    return GccEnt_QualifiedCirc(Circle,GccEnt_enclosed);
  }

GccEnt_QualifiedCirc
  GccEnt::Outside(const gp_Circ2d& Circle) {
    return GccEnt_QualifiedCirc(Circle,GccEnt_outside);
  }

GccEnt_QualifiedLin
  GccEnt::Outside(const gp_Lin2d& Line) {
    return GccEnt_QualifiedLin(Line,GccEnt_outside);
  }
