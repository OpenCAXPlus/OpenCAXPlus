// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Geom2dGcc_CurveToolGeo_HeaderFile
#define _Geom2dGcc_CurveToolGeo_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <GeomAbs_CurveType.hxx>
#include <Standard_Real.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Boolean.hxx>
class Adaptor3d_OffsetCurve;
class gp_Lin2d;
class gp_Circ2d;
class gp_Elips2d;
class gp_Parab2d;
class gp_Hypr2d;
class gp_Pnt2d;
class gp_Vec2d;



class Geom2dGcc_CurveToolGeo 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT static   GeomAbs_CurveType TheType (const Adaptor3d_OffsetCurve& C) ;
  
  //! Returns the Lin2d from gp corresponding to the curve C.
  //! This method is called only when TheType returns
  //! IntCurve_Lin.
  Standard_EXPORT static   gp_Lin2d Line (const Adaptor3d_OffsetCurve& C) ;
  
  //! Returns the Circ2d from gp corresponding to the curve C.
  //! This method is called only when TheType returns
  //! IntCurve_Cir.
  Standard_EXPORT static   gp_Circ2d Circle (const Adaptor3d_OffsetCurve& C) ;
  
  //! Returns the Elips2d from gp corresponding to the curve C.
  //! This method is called only when TheType returns
  //! IntCurve_Eli.
  Standard_EXPORT static   gp_Elips2d Ellipse (const Adaptor3d_OffsetCurve& C) ;
  
  //! Returns the Parab2d from gp corresponding to the curve C.
  //! This method is called only when TheType returns
  //! IntCurve_Prb.
  Standard_EXPORT static   gp_Parab2d Parabola (const Adaptor3d_OffsetCurve& C) ;
  
  //! Returns the Hypr2d from gp corresponding to the curve C.
  //! This method is called only when TheType returns
  //! IntCurve_Hpr.
  Standard_EXPORT static   gp_Hypr2d Hyperbola (const Adaptor3d_OffsetCurve& C) ;
  
  Standard_EXPORT static   Standard_Real FirstParameter (const Adaptor3d_OffsetCurve& C) ;
  
  Standard_EXPORT static   Standard_Real LastParameter (const Adaptor3d_OffsetCurve& C) ;
  
  Standard_EXPORT static   Standard_Real EpsX (const Adaptor3d_OffsetCurve& C, const Standard_Real Tol) ;
  
  Standard_EXPORT static   Standard_Integer NbSamples (const Adaptor3d_OffsetCurve& C) ;
  
  Standard_EXPORT static   gp_Pnt2d Value (const Adaptor3d_OffsetCurve& C, const Standard_Real X) ;
  
  Standard_EXPORT static   void D1 (const Adaptor3d_OffsetCurve& C, const Standard_Real U, gp_Pnt2d& P, gp_Vec2d& T) ;
  
  Standard_EXPORT static   void D2 (const Adaptor3d_OffsetCurve& C, const Standard_Real U, gp_Pnt2d& P, gp_Vec2d& T, gp_Vec2d& N) ;
  
  Standard_EXPORT static   Standard_Boolean IsComposite (const Adaptor3d_OffsetCurve& C) ;
  
  //! Outputs the number of interval of continuity C1 of
  //! the curve
  //! used if Type == Composite.
  Standard_EXPORT static   Standard_Integer GetIntervals (const Adaptor3d_OffsetCurve& C) ;
  
  //! Outputs the bounds of interval of index <Index>
  //! used if Type == Composite.
  Standard_EXPORT static   void GetInterval (const Adaptor3d_OffsetCurve& C, const Standard_Integer Index, Standard_Real& U1, Standard_Real& U2) ;
  
  //! Set the current valid interval of index <Index>
  //! inside which the computations will be done
  //! (used if Type == Composite).
  Standard_EXPORT static   void SetCurrentInterval (Adaptor3d_OffsetCurve& C, const Standard_Integer Index) ;




protected:





private:





};







#endif // _Geom2dGcc_CurveToolGeo_HeaderFile
