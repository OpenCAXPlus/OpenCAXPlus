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

#include <StepBasic_ConversionBasedUnitAndRatioUnit.ixx>
#include <StepBasic_ConversionBasedUnit.hxx>
#include <StepBasic_RatioUnit.hxx>


StepBasic_ConversionBasedUnitAndRatioUnit::StepBasic_ConversionBasedUnitAndRatioUnit ()
{
}

void StepBasic_ConversionBasedUnitAndRatioUnit::Init(const Handle(StepBasic_DimensionalExponents)& aDimensions)
{
  StepBasic_NamedUnit::Init(aDimensions);
}


void StepBasic_ConversionBasedUnitAndRatioUnit::Init(const Handle(StepBasic_DimensionalExponents)& aDimensions,
						     const Handle(TCollection_HAsciiString)& aName,
						     const Handle(StepBasic_MeasureWithUnit)& aConversionFactor)
{
  // --- ANDOR componant fields ---
  StepBasic_ConversionBasedUnit::Init(aDimensions, aName, aConversionFactor);
	
  // --- ANDOR componant fields ---
  ratioUnit = new StepBasic_RatioUnit();
  ratioUnit->Init(aDimensions);
}


void StepBasic_ConversionBasedUnitAndRatioUnit::SetRatioUnit(const Handle(StepBasic_RatioUnit)& aRatioUnit)
{
  ratioUnit = aRatioUnit;
}

Handle(StepBasic_RatioUnit) StepBasic_ConversionBasedUnitAndRatioUnit::RatioUnit() const
{
  return ratioUnit;
}
