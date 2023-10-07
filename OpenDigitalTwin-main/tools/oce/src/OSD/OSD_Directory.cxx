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

#ifndef WNT

#include <OSD_Directory.ixx>
#include <OSD_WhoAmI.hxx>
#include <OSD_Protection.hxx>

#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

const OSD_WhoAmI Iam = OSD_WDirectory;


OSD_Directory::OSD_Directory():OSD_FileNode() {
}

OSD_Directory::OSD_Directory(const OSD_Path& Name):OSD_FileNode(Name){
}

// Create physically a directory

void OSD_Directory::Build(const OSD_Protection& Protect){
int status;
Standard_Integer internal_prot;
TCollection_AsciiString aBuffer;

 internal_prot = Protect.Internal();
 myPath.SystemName(aBuffer);
 umask ( 0 );
 status = mkdir (aBuffer.ToCString(), (mode_t)internal_prot);
 if (status == -1) 
  if (errno != EEXIST) {
   Standard_PCharacter err_message;

   err_message = new Standard_Character [255];
   sprintf (err_message,
	    "OSD_Directory::Build Directory \"%s\"",
	    aBuffer.ToCString());
   
   myError.SetValue(errno, Iam, err_message);
   delete err_message;
  }
}

OSD_Directory OSD_Directory::BuildTemporary(){
OSD_Protection          Protect;
OSD_Directory           aDirectoryToReturn;
Standard_Integer        internal_prot;
char dirfmt[] = "/tmp/oceXXXXXX";

 internal_prot = Protect.Internal();

 umask ( (mode_t)(~internal_prot) ); // indirectly impose internal_prot on mkdtemp call
 char *name = mkdtemp(dirfmt);
 umask(0); // reset umask
 unlink(name);//Destroys link but directory still exists while 
              //current process lives.

 TCollection_AsciiString aString (name);
 aDirectoryToReturn.SetPath ( aString );
 return aDirectoryToReturn;

}

#else

//------------------------------------------------------------------------
//-------------------  WNT Sources of OSD_Diretory -----------------------
//------------------------------------------------------------------------

#include <OSD_Directory.hxx>
#include <OSD_Protection.hxx>
#include <Standard_ProgramError.hxx>
#include <TCollection_ExtendedString.hxx>

#include <OSD_WNT_1.hxx>

#include <stdio.h>

#ifndef _INC_TCHAR
# include <tchar.h>
#endif  // _INC_TCHAR

#ifdef _UNICODE
# define tctmpnam _wtmpnam
#else
# define tctmpnam tmpnam
#endif  // _UNICODE

void _osd_wnt_set_error ( OSD_Error&, OSD_WhoAmI, ... );

OSD_Directory :: OSD_Directory () {

}  // end constructor ( 1 )

OSD_Directory :: OSD_Directory ( const OSD_Path& Name ) :
                 OSD_FileNode ( Name ) {

}  // end constructor ( 2 )

void OSD_Directory :: Build (const OSD_Protection& Protect ) {

 TCollection_AsciiString dirName;

 myPath.SystemName ( dirName );

 if (  dirName.IsEmpty ()  )

  Standard_ProgramError :: Raise (
                            TEXT( "OSD_Directory :: Build (): incorrect call - no directory name" )
                           );
 TCollection_ExtendedString dirNameW(dirName);
 if (  Exists () || CreateDirectoryW ( (const wchar_t*) dirNameW.ToExtString (), NULL )  )

  SetProtection ( Protect );

 else

  _osd_wnt_set_error ( myError, OSD_WDirectory );

}  // end OSD_Directory :: Build

OSD_Directory OSD_Directory :: BuildTemporary () {

 OSD_Directory           retVal;
 OSD_Protection          prt;
 OSD_Path                dirPath (  tctmpnam ( NULL )  );

 retVal.SetPath ( dirPath );
 retVal.Build ( prt );                            

 return retVal;

}  // end OSD_Directory :: BuildTemporary

#endif
