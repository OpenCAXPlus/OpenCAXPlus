// Created on: 1994-10-03
// Created by: Laurent BOURESCHE
// Copyright (c) 1994-1999 Matra Datavision
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

#include <Standard_Boolean.hxx>

#ifndef __Blend_API
# if defined(WNT) && !defined(HAVE_NO_DLL)
#  ifdef __Blend_DLL
#   define __Blend_API __declspec( dllexport )
#  else
#   define __Blend_API __declspec( dllimport )
#  endif /*__Blend_DLL*/
# else
#  define __Blend_API
# endif /*WNT*/
#endif /*__Blend_API*/

//*************************************************
// recuperation des surfaces des conges approximes.
//*************************************************

static Standard_Boolean Blend_traceDRAWSECT = Standard_False;
__Blend_API extern void Blend_SettraceDRAWSECT(const Standard_Boolean b) 
{ Blend_traceDRAWSECT = b; }
__Blend_API extern Standard_Boolean Blend_GettraceDRAWSECT() 
{ return Blend_traceDRAWSECT; }

//*************************************************
// Contexte sans test de deflexion
//*************************************************

static Standard_Boolean Blend_contextNOTESTDEFL = Standard_False;
__Blend_API extern void Blend_SetcontextNOTESTDEFL(const Standard_Boolean b) 
{ Blend_contextNOTESTDEFL = b; }
__Blend_API extern Standard_Boolean Blend_GetcontextNOTESTDEFL() 
{ return Blend_contextNOTESTDEFL; }
