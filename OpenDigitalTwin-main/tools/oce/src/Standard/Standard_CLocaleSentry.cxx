// Created on: 2013-01-17
// Created by: Kirill GAVRILOV
// Copyright (c) 2013-2014 OPEN CASCADE SAS
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

#include <Standard_CLocaleSentry.hxx>

#include <Standard_TypeDef.hxx>

#include <cstring>

#if !defined(__ANDROID__)

#if defined(__BORLANDC__)
#include <mem.h>
#endif

namespace
{

  //! CLocalePtr - static object representing C locale
  class CLocalePtr
  {
  public:

    CLocalePtr()
    #ifdef HAVE_XLOCALE_H
    : myLocale (newlocale (LC_ALL_MASK, "C", NULL))
    #elif defined(_WIN32) && !defined(__BORLANDC__) && !defined(__MINGW32__)
    : myLocale (_create_locale (LC_ALL, "C"))
    #else
    : myLocale (NULL)
    #endif
    {}

    ~CLocalePtr()
    {
    #ifdef HAVE_XLOCALE_H
      freelocale (myLocale);
    #elif defined(_WIN32) && !defined(__BORLANDC__) && !defined(__MINGW32__)
      _free_locale (myLocale);
    #endif
    }

  public:

    Standard_CLocaleSentry::clocale_t myLocale;

  };

  static CLocalePtr theCLocale;

}

// =======================================================================
// function : GetCLocale
// purpose  :
// =======================================================================
Standard_CLocaleSentry::clocale_t Standard_CLocaleSentry::GetCLocale()
{
  return theCLocale.myLocale;
}

// =======================================================================
// function : Standard_CLocaleSentry
// purpose  :
// =======================================================================
Standard_CLocaleSentry::Standard_CLocaleSentry()
#ifdef HAVE_XLOCALE_H
: myPrevLocale (uselocale (theCLocale.myLocale)) // switch to C locale within this thread only using xlocale API
#else
: myPrevLocale (setlocale (LC_ALL, 0))
#if defined(_MSC_VER) && (_MSC_VER > 1400)
, myPrevTLocaleState (_configthreadlocale (_ENABLE_PER_THREAD_LOCALE))
#endif
#endif
{
#if !defined(HAVE_XLOCALE_H)
  const char* aPrevLocale = (const char* )myPrevLocale;
  if (myPrevLocale == NULL
   || (aPrevLocale[0] == 'C' && aPrevLocale[1] == '\0'))
  {
    myPrevLocale = NULL; // already C locale
    return;
  }
  // copy string as following setlocale calls may invalidate returned pointer
  Standard_Size aLen = std::strlen (aPrevLocale) + 1;
  myPrevLocale = new char[aLen];
  memcpy (myPrevLocale, aPrevLocale, aLen);

  setlocale (LC_ALL, "C");
#endif
}

// =======================================================================
// function : ~Standard_CLocaleSentry
// purpose  :
// =======================================================================
Standard_CLocaleSentry::~Standard_CLocaleSentry()
{
#if defined(HAVE_XLOCALE_H)
  uselocale ((locale_t )myPrevLocale);
#else
  if (myPrevLocale != NULL)
  {
    const char* aPrevLocale = (const char* )myPrevLocale;
    setlocale (LC_ALL, aPrevLocale);
    delete[] aPrevLocale;
  }
#if defined(_MSC_VER) && (_MSC_VER > 1400)
  if (myPrevTLocaleState != _ENABLE_PER_THREAD_LOCALE)
  {
    _configthreadlocale (myPrevTLocaleState);
  }
#endif
#endif
}

#endif // __ANDROID__
