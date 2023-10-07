// Created on: 2000-05-24
// Created by: data exchange team
// Copyright (c) 2000-2014 OPEN CASCADE SAS
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

#include <XCAFApp_Application.ixx>
#include <TPrsStd_DriverTable.hxx>
#include <XCAFPrs_Driver.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDF_Label.hxx>

//=======================================================================
//function : GetApplication
//purpose  : 
//=======================================================================

Handle(XCAFApp_Application) XCAFApp_Application::GetApplication() 
{
  static Handle(XCAFApp_Application) locApp;
  if ( locApp.IsNull() ) locApp = new XCAFApp_Application;
  return locApp;
}

//=======================================================================
//function : XCAFApp_Application
//purpose  : 
//=======================================================================

XCAFApp_Application::XCAFApp_Application()
{
  // register driver for presentation
  Handle(TPrsStd_DriverTable) table  = TPrsStd_DriverTable::Get();
  table->AddDriver (XCAFPrs_Driver::GetID(), new XCAFPrs_Driver);
}

//=======================================================================
//function : Formats
//purpose  : 
//=======================================================================

void XCAFApp_Application::Formats(TColStd_SequenceOfExtendedString& Formats) 
{
  Formats.Append(TCollection_ExtendedString ("MDTV-XCAF"));  
  Formats.Append(TCollection_ExtendedString ("XmlXCAF"));
  Formats.Append(TCollection_ExtendedString ("XmlOcaf"));
  Formats.Append(TCollection_ExtendedString ("MDTV-Standard"));
}

//=======================================================================
//function : ResourcesName
//purpose  : 
//=======================================================================

Standard_CString XCAFApp_Application::ResourcesName() 
{
  return Standard_CString("XCAF");
//  return Standard_CString("Standard");
}

//=======================================================================
//function : InitDocument
//purpose  : 
//=======================================================================

void XCAFApp_Application::InitDocument(const Handle(TDocStd_Document)& aDoc) const
{
  XCAFDoc_DocumentTool::Set(aDoc->Main());
}
