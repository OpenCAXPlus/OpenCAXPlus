// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Standard_TooManyUsers_HeaderFile
#define _Standard_TooManyUsers_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Standard_DefineException.hxx>
#include <Standard_SStream.hxx>
#include <Standard_LicenseError.hxx>
#include <Handle_Standard_TooManyUsers.hxx>

#if !defined No_Exception && !defined No_Standard_TooManyUsers
  #define Standard_TooManyUsers_Raise_if(CONDITION, MESSAGE) \
  if (CONDITION) Standard_TooManyUsers::Raise(MESSAGE);
#else
  #define Standard_TooManyUsers_Raise_if(CONDITION, MESSAGE)
#endif

DEFINE_STANDARD_EXCEPTION(Standard_TooManyUsers, Standard_LicenseError)

#endif // _Standard_TooManyUsers_HeaderFile
