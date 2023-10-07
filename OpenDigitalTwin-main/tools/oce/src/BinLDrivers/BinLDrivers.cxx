// Created on: 2002-10-29
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

#include <BinLDrivers.ixx>
#include <BinMDF.hxx>
#include <BinMDataStd.hxx>
#include <BinMFunction.hxx>
//#include <BinMNaming.hxx>
#include <BinMDocStd.hxx>
#include <Standard_Failure.hxx>
#include <BinLDrivers_DocumentStorageDriver.hxx>
#include <BinLDrivers_DocumentRetrievalDriver.hxx>
#include <Plugin_Macro.hxx>

static Standard_GUID BinLStorageDriver  ("13a56835-8269-11d5-aab2-0050044b1af1");
static Standard_GUID BinLRetrievalDriver("13a56836-8269-11d5-aab2-0050044b1af1");
#define CURRENT_DOCUMENT_VERSION 7

//=======================================================================
//function : Factory
//purpose  : PLUGIN FACTORY
//=======================================================================
Handle(Standard_Transient) BinLDrivers::Factory(const Standard_GUID& theGUID)
{
  if (theGUID == BinLStorageDriver)
  {
#ifdef OCCT_DEBUG
    cout << "BinLDrivers : Storage Plugin" << endl;
#endif
    static Handle(BinLDrivers_DocumentStorageDriver) model_sd =
      new BinLDrivers_DocumentStorageDriver;
    return model_sd;
  }

  if (theGUID == BinLRetrievalDriver)
  {
#ifdef OCCT_DEBUG
    cout << "BinLDrivers : Retrieval Plugin" << endl;
#endif
    static Handle(BinLDrivers_DocumentRetrievalDriver) model_rd =
      new BinLDrivers_DocumentRetrievalDriver;
    return model_rd;
  }

  Standard_Failure::Raise ("BinLDrivers : unknown GUID");
  return NULL;
}

//=======================================================================
//function : AttributeDrivers
//purpose  :
//=======================================================================

Handle(BinMDF_ADriverTable) BinLDrivers::AttributeDrivers 
                         (const Handle(CDM_MessageDriver)& aMsgDrv)
{
  Handle(BinMDF_ADriverTable) aTable = new BinMDF_ADriverTable;

  BinMDF        ::AddDrivers (aTable, aMsgDrv);
  BinMDataStd   ::AddDrivers (aTable, aMsgDrv);
  BinMFunction  ::AddDrivers (aTable, aMsgDrv);
  BinMDocStd    ::AddDrivers (aTable, aMsgDrv);

//  BinMNaming    ::AddDrivers (aTable, aMsgDrv);
//  BinMPrsStd    ::AddDrivers (aTable, aMsgDrv);
  return aTable;
}

//=======================================================================
//function : StorageVersion
//purpose  : 
//=======================================================================

TCollection_AsciiString BinLDrivers::StorageVersion()
{
  TCollection_AsciiString aVersionStr (CURRENT_DOCUMENT_VERSION);
  return aVersionStr;
}

#ifdef _MSC_VER
#pragma warning(disable:4190) /* disable warning on C++ type returned by C function; should be OK for C++ usage */
#endif
PLUGIN(BinLDrivers)
