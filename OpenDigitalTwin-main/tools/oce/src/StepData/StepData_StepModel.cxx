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

#include <StepData_StepModel.ixx>
#include <Standard_NoSuchObject.hxx>
#include <StepData_Protocol.hxx>
#include <StepData_StepWriter.hxx>

#include <StepData.hxx>
#include <Interface_CopyTool.hxx>
#include <Interface_GeneralLib.hxx>
#include <Interface_ShareTool.hxx>
#include <Interface_GeneralModule.hxx>
#include <Interface_Macros.hxx>
#include <stdio.h>


// Entete de fichier : liste d entites

StepData_StepModel::StepData_StepModel ()  {  }


Handle(Standard_Transient) StepData_StepModel::Entity
(const Standard_Integer num) const
{  return Value(num);  }      // nom plus joli

void StepData_StepModel::GetFromAnother
(const Handle(Interface_InterfaceModel)& other)
{
  theheader.Clear();
  DeclareAndCast(StepData_StepModel,another,other);
  if (another.IsNull()) return;
  Interface_EntityIterator iter = another->Header();
  //  recopier le header. Attention, header distinct du contenu ...
  Interface_CopyTool TC (this,StepData::HeaderProtocol());
  for (; iter.More(); iter.Next()) {
    Handle(Standard_Transient) newhead;
    if (!TC.Copy(iter.Value(),newhead,Standard_False,Standard_False)) continue;
    if (!newhead.IsNull()) theheader.Append(newhead);
  }
}

Handle(Interface_InterfaceModel) StepData_StepModel::NewEmptyModel () const
{  return new StepData_StepModel;  }


Interface_EntityIterator StepData_StepModel::Header () const
{
  Interface_EntityIterator iter;
  theheader.FillIterator(iter);
  return iter;
}

Standard_Boolean StepData_StepModel::HasHeaderEntity
(const Handle(Standard_Type)& atype) const
{  return (theheader.NbTypedEntities(atype) == 1);  }

Handle(Standard_Transient) StepData_StepModel::HeaderEntity
(const Handle(Standard_Type)& atype) const
{  return theheader.TypedEntity(atype);  }


//   Remplissage du Header

void StepData_StepModel::ClearHeader ()
{  theheader.Clear();  }


void StepData_StepModel::AddHeaderEntity
(const Handle(Standard_Transient)& ent)
{  theheader.Append(ent);  }


void StepData_StepModel::VerifyCheck(Handle(Interface_Check)& ach) const
{
  Interface_GeneralLib lib(StepData::HeaderProtocol());
  Interface_ShareTool sh(this,StepData::HeaderProtocol());
  Handle(Interface_GeneralModule) module;  Standard_Integer CN;
  for (Interface_EntityIterator iter = Header(); iter.More(); iter.Next()) {
    Handle(Standard_Transient) head = iter.Value();
    if (!lib.Select(head,module,CN)) continue;
    module->CheckCase(CN,head,sh,ach);
  }
}


void StepData_StepModel::DumpHeader
(const Handle(Message_Messenger)& S, const Standard_Integer /*level*/) const
{
  //  NB : level n est pas utilise

  Handle(StepData_Protocol) stepro = StepData::HeaderProtocol();
  Standard_Boolean iapro = !stepro.IsNull();
  if (!iapro) S<<" -- WARNING : StepModel DumpHeader, Protocol not defined\n";

  Interface_EntityIterator iter = Header();
  Standard_Integer nb = iter.NbEntities();
  S << " --  Step Model Header : " <<iter.NbEntities() << " Entities :\n";
  for (iter.Start(); iter.More(); iter.Next()) {
    S << "  "  << iter.Value()->DynamicType()->Name() << "\n";
  }
  if (!iapro || nb == 0) return;
  S << " --  --        STEP MODEL    HEADER  CONTENT      --  --" << endl;
  S << " --   Dumped with Protocol : " << stepro->DynamicType()->Name()
    << "   --"<<endl;

  Standard_SStream aSStream;
  StepData_StepWriter SW(this);
  SW.SendModel(stepro,Standard_True);    // envoi HEADER seul
  SW.Print(aSStream);
  S << aSStream.str().c_str();
}


void  StepData_StepModel::ClearLabels ()
{  theidnums.Nullify();  }

void  StepData_StepModel::SetIdentLabel
(const Handle(Standard_Transient)& ent, const Standard_Integer ident)
{
  Standard_Integer num = Number(ent);
  if (!num) 
    return;
  if(theidnums.IsNull())
  {
    theidnums = new TColStd_HArray1OfInteger(1, NbEntities());
    theidnums->Init(0);
  }
  theidnums->SetValue(num,ident);

}

Standard_Integer  StepData_StepModel::IdentLabel
(const Handle(Standard_Transient)& ent) const
{
  if(theidnums.IsNull())
    return 0;
  Standard_Integer num = Number(ent);
  return (!num ? 0 : theidnums->Value(num));
 }

void  StepData_StepModel::PrintLabel
(const Handle(Standard_Transient)& ent, const Handle(Message_Messenger)& S) const
{
  Standard_Integer num = (theidnums.IsNull() ? 0 : Number(ent));
  Standard_Integer  nid = (!num ? 0 : theidnums->Value(num));
  if      (nid > 0) S<<"#"<<nid;
  else if (num > 0) S<<"(#"<<num<<")";
  else              S<<"(#0..)";
}

Handle(TCollection_HAsciiString) StepData_StepModel::StringLabel
(const Handle(Standard_Transient)& ent) const
{
  Handle(TCollection_HAsciiString) label;
  char text[20];
  Standard_Integer num = (theidnums.IsNull() ? 0 : Number(ent));
  Standard_Integer  nid = (!num ? 0 : theidnums->Value(num));

  if      (nid > 0) sprintf (text, "#%d",nid);
  else if (num > 0) sprintf (text, "(#%d)",num);
  else              sprintf (text, "(#0..)");

  label = new TCollection_HAsciiString(text);
  return label;
}
