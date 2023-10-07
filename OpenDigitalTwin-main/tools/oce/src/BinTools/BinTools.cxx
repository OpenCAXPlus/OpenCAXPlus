// Created on: 2004-05-18
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

#include <BinTools.ixx>
#include <FSD_FileHeader.hxx>
#include <Storage_StreamTypeMismatchError.hxx>

//=======================================================================
//function : PutBool
//purpose  : 
//=======================================================================

Standard_OStream& BinTools::PutBool(Standard_OStream& OS, const Standard_Boolean aValue)
{
  OS.put((Standard_Byte)aValue);
  return OS;
}


//=======================================================================
//function : PutInteger
//purpose  : 
//=======================================================================

Standard_OStream& BinTools::PutInteger(Standard_OStream& OS, const Standard_Integer aValue)
{
  Standard_Integer anIntValue = aValue;
#if DO_INVERSE
      anIntValue = InverseInt (aValue);
#endif
  OS.write((char*)&anIntValue, sizeof(Standard_Integer));  
  return OS;
}


//=======================================================================
//function : PutReal
//purpose  : 
//=======================================================================

Standard_OStream& BinTools::PutReal(Standard_OStream& OS, const Standard_Real aValue)
{
  Standard_Real aRValue = aValue;
#if DO_INVERSE
      aRValue = InverseReal (aValue);
#endif
  OS.write((char*)&aRValue, sizeof(Standard_Real));  
  return OS;
}

//=======================================================================
//function : PutExtChar
//purpose  : 
//=======================================================================

Standard_OStream& BinTools::PutExtChar(Standard_OStream& OS, const Standard_ExtCharacter aValue)
{
  Standard_ExtCharacter aSValue = aValue;
#if DO_INVERSE
      aSValue = InverseExtChar (aValue);
#endif
  OS.write((char*)&aSValue, sizeof(Standard_ExtCharacter));  
  return OS;
}
//=======================================================================
//function : GetReal
//purpose  : 
//=======================================================================

Standard_IStream& BinTools::GetReal(Standard_IStream& IS, Standard_Real& aValue)
{
  if(!IS.read ((char*)&aValue, sizeof(Standard_Real)))
    Storage_StreamTypeMismatchError::Raise();
#if DO_INVERSE
  aValue = InverseReal (aValue);
#endif
  return IS;
}

//=======================================================================
//function : GetInteger
//purpose  : 
//=======================================================================

Standard_IStream& BinTools::GetInteger(Standard_IStream& IS, Standard_Integer& aValue)
{
  if(!IS.read ((char*)&aValue, sizeof(Standard_Integer)))
    Storage_StreamTypeMismatchError::Raise();;
#if DO_INVERSE
  aValue = InverseInt (aValue);
#endif
  return IS;
}

//=======================================================================
//function : GetExtChar
//purpose  : 
//=======================================================================

Standard_IStream& BinTools::GetExtChar(Standard_IStream& IS, Standard_ExtCharacter& theValue)
{
  if(!IS.read ((char*)&theValue, sizeof(Standard_ExtCharacter)))
    Storage_StreamTypeMismatchError::Raise();;
#if DO_INVERSE
  theValue = InverseExtChar (theValue);
#endif
  return IS;
}

//=======================================================================
//function : GetBool
//purpose  : 
//=======================================================================

Standard_IStream& BinTools::GetBool(Standard_IStream& IS, Standard_Boolean& aValue)
{
  aValue = (0 != IS.get());
  return IS;
}
