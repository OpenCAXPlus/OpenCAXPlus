// Copyright (c) 1998-1999 Matra Datavision
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

#include <Storage_DefaultCallBack.ixx>

Storage_DefaultCallBack::Storage_DefaultCallBack()
{
}

Handle(Standard_Persistent) Storage_DefaultCallBack::New() const
{
  Handle(Standard_Persistent) res = new Standard_Persistent;

  return res;
}

void Storage_DefaultCallBack::Add(const Handle(Standard_Persistent)&, const Handle(Storage_Schema)&) const
{
}

void Storage_DefaultCallBack::Write(const Handle(Standard_Persistent)&,Storage_BaseDriver&,const Handle(Storage_Schema)&) const
{
}

void Storage_DefaultCallBack::Read(const Handle(Standard_Persistent)&,Storage_BaseDriver& f,const Handle(Storage_Schema)&) const
{
  f.SkipObject();
}
