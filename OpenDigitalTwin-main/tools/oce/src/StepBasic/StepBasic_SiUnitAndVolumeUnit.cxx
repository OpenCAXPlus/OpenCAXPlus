// Created on: 1999-10-12
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

#include <StepBasic_SiUnitAndVolumeUnit.ixx>

StepBasic_SiUnitAndVolumeUnit::StepBasic_SiUnitAndVolumeUnit()
{
}

void StepBasic_SiUnitAndVolumeUnit::SetVolumeUnit(const Handle(StepBasic_VolumeUnit)& aVolumeUnit)
{
  volumeUnit = aVolumeUnit;
}

Handle(StepBasic_VolumeUnit) StepBasic_SiUnitAndVolumeUnit::VolumeUnit() const
{
  return volumeUnit;
}

void StepBasic_SiUnitAndVolumeUnit::SetDimensions(const Handle(StepBasic_DimensionalExponents)& aDimensions)
{
  StepBasic_NamedUnit::SetDimensions(aDimensions);
}

Handle(StepBasic_DimensionalExponents) StepBasic_SiUnitAndVolumeUnit::Dimensions() const
{
  return StepBasic_NamedUnit::Dimensions();
}
