// Created on: 2002-10-30
// Created by: Michael SAZONOV
// Copyright (c) 2002-2014 OPEN CASCADE SAS
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

#include <BinMDataStd_RealDriver.ixx>
#include <TDataStd_Real.hxx>

//=======================================================================
//function : BinMDataStd_RealDriver
//purpose  : Constructor
//=======================================================================

BinMDataStd_RealDriver::BinMDataStd_RealDriver 
                        (const Handle(CDM_MessageDriver)& theMsgDriver)
     : BinMDF_ADriver (theMsgDriver, STANDARD_TYPE(TDataStd_Real)->Name())
{
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) BinMDataStd_RealDriver::NewEmpty() const
{
  return new TDataStd_Real();
}

//=======================================================================
//function : Paste
//purpose  : persistent -> transient (retrieve)
//=======================================================================

Standard_Boolean BinMDataStd_RealDriver::Paste
                                (const BinObjMgt_Persistent&  theSource,
                                 const Handle(TDF_Attribute)& theTarget,
                                 BinObjMgt_RRelocationTable&  ) const
{
  Handle(TDataStd_Real) anAtt= Handle(TDataStd_Real)::DownCast(theTarget);
  Standard_Real aValue=0.0;
  Standard_Boolean ok = theSource >> aValue;
  if (ok)
    anAtt->Set(aValue);
  return ok;
}

//=======================================================================
//function : Paste
//purpose  : transient -> persistent (store)
//=======================================================================

void BinMDataStd_RealDriver::Paste (const Handle(TDF_Attribute)& theSource,
                                    BinObjMgt_Persistent&        theTarget,
                                    BinObjMgt_SRelocationTable&  ) const
{
  Handle(TDataStd_Real) anAtt= Handle(TDataStd_Real)::DownCast(theSource);
  theTarget << anAtt->Get();
}
