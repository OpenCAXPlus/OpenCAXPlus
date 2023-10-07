// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BinTools_SurfaceSet_HeaderFile
#define _BinTools_SurfaceSet_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TColStd_IndexedMapOfTransient.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Geom_Surface.hxx>
#include <Standard_OStream.hxx>
#include <Standard_IStream.hxx>
class Standard_OutOfRange;
class Geom_Surface;


//! Stores a set of Surfaces from Geom in binary format.
class BinTools_SurfaceSet 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Returns an empty set of Surfaces.
  Standard_EXPORT BinTools_SurfaceSet();
  
  //! Clears the content of the set.
  Standard_EXPORT   void Clear() ;
  
  //! Incorporate a new Surface in the  set and returns
  //! its index.
  Standard_EXPORT   Standard_Integer Add (const Handle(Geom_Surface)& S) ;
  
  //! Returns the Surface of index <I>.
  Standard_EXPORT   Handle(Geom_Surface) Surface (const Standard_Integer I)  const;
  
  //! Returns the index of <L>.
  Standard_EXPORT   Standard_Integer Index (const Handle(Geom_Surface)& S)  const;
  
  //! Writes the content of  me  on the stream <OS> in
  //! binary format that can be read back by Read.
  Standard_EXPORT   void Write (Standard_OStream& OS)  const;
  
  //! Reads the content of me from the  stream  <IS>. me
  //! is first cleared.
  Standard_EXPORT   void Read (Standard_IStream& IS) ;
  
  //! Dumps the surface on the stream in binary
  //! format that can be read back.
  Standard_EXPORT static   void WriteSurface (const Handle(Geom_Surface)& S, Standard_OStream& OS) ;
  
  //! Reads the surface  from  the stream.  The  surface  is
  //! assumed   to have  been  written  with  the Write
  //! method.
  Standard_EXPORT static   Standard_IStream& ReadSurface (Standard_IStream& IS, Handle(Geom_Surface)& S) ;




protected:





private:



  TColStd_IndexedMapOfTransient myMap;


};







#endif // _BinTools_SurfaceSet_HeaderFile
