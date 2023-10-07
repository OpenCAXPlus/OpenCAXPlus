// Created on: 2001-07-09
// Created by: Julia DOROVSKIKH
// Copyright (c) 2001-2014 OPEN CASCADE SAS
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

#include <XmlDrivers_DocumentRetrievalDriver.ixx>

#include <XmlDrivers.hxx>
#include <XmlMDataStd.hxx>
#include <XmlMNaming.hxx>
#include <XmlObjMgt_Element.hxx>
#include <XmlMNaming_NamedShapeDriver.hxx>
#include <TNaming_NamedShape.hxx>
#include <XmlMDF_ADriver.hxx>
#include <CDM_MessageDriver.hxx>
//=======================================================================
//function : XmlDrivers_DocumentRetrievalDriver
//purpose  : Constructor
//=======================================================================
XmlDrivers_DocumentRetrievalDriver::XmlDrivers_DocumentRetrievalDriver()
{
}

//=======================================================================
//function : AttributeDrivers
//purpose  : 
//=======================================================================
Handle(XmlMDF_ADriverTable) XmlDrivers_DocumentRetrievalDriver::AttributeDrivers
       (const Handle(CDM_MessageDriver)& theMessageDriver) 
{
  return XmlDrivers::AttributeDrivers (theMessageDriver);
}

//=======================================================================
//function : PropagateDocumentVersion
//purpose  : 
//=======================================================================
void XmlDrivers_DocumentRetrievalDriver::PropagateDocumentVersion(
                                   const Standard_Integer theDocVersion )
{
  XmlMDataStd::SetDocumentVersion(theDocVersion);
  XmlMNaming::SetDocumentVersion(theDocVersion);
}

//=======================================================================
//function : ReadShapeSection
//purpose  : Implementation of ReadShapeSection
//=======================================================================
Handle(XmlMDF_ADriver) XmlDrivers_DocumentRetrievalDriver::ReadShapeSection(
                               const XmlObjMgt_Element&       theElement,
                               const Handle(CDM_MessageDriver)& theMsgDriver)
{
  if (myDrivers.IsNull()) myDrivers = AttributeDrivers (theMsgDriver);
  Handle(XmlMNaming_NamedShapeDriver) aNamedShapeDriver;
  if (myDrivers -> GetDriver (STANDARD_TYPE(TNaming_NamedShape),
                              aNamedShapeDriver))
    aNamedShapeDriver -> ReadShapeSection (theElement);
  return aNamedShapeDriver;
}

//=======================================================================
//function : ShapeSetCleaning
//purpose  : definition of ShapeSetCleaning
//=======================================================================
void XmlDrivers_DocumentRetrievalDriver::ShapeSetCleaning(
			      const Handle(XmlMDF_ADriver)& theDriver) 
{
  Handle(XmlMNaming_NamedShapeDriver) aNamedShapeDriver = 
    Handle(XmlMNaming_NamedShapeDriver)::DownCast(theDriver);
  if (aNamedShapeDriver.IsNull() == Standard_False)
    aNamedShapeDriver -> Clear();
}


