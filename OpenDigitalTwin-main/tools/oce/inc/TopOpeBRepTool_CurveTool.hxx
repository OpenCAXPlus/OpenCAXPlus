// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TopOpeBRepTool_CurveTool_HeaderFile
#define _TopOpeBRepTool_CurveTool_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TopOpeBRepTool_GeomTool.hxx>
#include <TopOpeBRepTool_OutCurveType.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Real.hxx>
#include <Handle_Geom_Curve.hxx>
#include <Handle_Geom2d_Curve.hxx>
class TopOpeBRepTool_GeomTool;
class Geom_Curve;
class Geom2d_Curve;
class TopoDS_Shape;
class TColgp_Array1OfPnt;
class TColgp_Array1OfPnt2d;



class TopOpeBRepTool_CurveTool 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TopOpeBRepTool_CurveTool();
  
  Standard_EXPORT TopOpeBRepTool_CurveTool(const TopOpeBRepTool_OutCurveType OCT);
  
  Standard_EXPORT TopOpeBRepTool_CurveTool(const TopOpeBRepTool_GeomTool& GT);
  
  Standard_EXPORT   TopOpeBRepTool_GeomTool& ChangeGeomTool() ;
  
  Standard_EXPORT  const  TopOpeBRepTool_GeomTool& GetGeomTool()  const;
  
  Standard_EXPORT   void SetGeomTool (const TopOpeBRepTool_GeomTool& GT) ;
  
  //! Approximates curves.
  //! Returns False in the case of failure
  Standard_EXPORT   Standard_Boolean MakeCurves (const Standard_Real min, const Standard_Real max, const Handle(Geom_Curve)& C3D, const Handle(Geom2d_Curve)& PC1, const Handle(Geom2d_Curve)& PC2, const TopoDS_Shape& S1, const TopoDS_Shape& S2, Handle(Geom_Curve)& C3DN, Handle(Geom2d_Curve)& PC1N, Handle(Geom2d_Curve)& PC2N, Standard_Real& Tol3d, Standard_Real& Tol2d)  const;
  
  Standard_EXPORT static   Handle(Geom_Curve) MakeBSpline1fromPnt (const TColgp_Array1OfPnt& P) ;
  
  Standard_EXPORT static   Handle(Geom2d_Curve) MakeBSpline1fromPnt2d (const TColgp_Array1OfPnt2d& P) ;
  
  Standard_EXPORT static   Standard_Boolean IsProjectable (const TopoDS_Shape& S, const Handle(Geom_Curve)& C) ;
  
  Standard_EXPORT static   Handle(Geom2d_Curve) MakePCurveOnFace (const TopoDS_Shape& S, const Handle(Geom_Curve)& C, Standard_Real& TolReached2d, const Standard_Real first = 0.0, const Standard_Real last = 0.0) ;




protected:



  TopOpeBRepTool_GeomTool myGeomTool;


private:





};







#endif // _TopOpeBRepTool_CurveTool_HeaderFile
