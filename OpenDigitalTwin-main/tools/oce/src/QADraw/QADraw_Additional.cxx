// Created on: 2002-03-12
// Created by: QA Admin
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

#include <QABugs.hxx>
#include <QADraw.hxx>
#include <QADNaming.hxx>
//#if defined(WOKC40)
//#include <QADBRepNaming.hxx>
//#else
#include <QANewDBRepNaming.hxx>
//#endif
#include <QANCollection.hxx>

void QADraw::AdditionalCommands(Draw_Interpretor& theCommands)
{
  QABugs::Commands(theCommands);

  QADNaming::AllCommands(theCommands);
//#if defined(WOKC40)
//  QADBRepNaming::AllCommands(theCommands);
//#else
  QANewDBRepNaming::AllCommands(theCommands);
//#endif
  QANCollection::Commands(theCommands);

  return;
}
