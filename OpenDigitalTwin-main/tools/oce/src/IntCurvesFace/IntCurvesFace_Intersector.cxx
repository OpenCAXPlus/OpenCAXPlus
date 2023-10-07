// Created on: 1996-06-03
// Created by: Laurent BUCHARD
// Copyright (c) 1996-1999 Matra Datavision
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

#define OPTIMISATION 1 


#include <IntCurvesFace_Intersector.ixx>

#include <IntCurveSurface_ThePolyhedronToolOfHInter.hxx>
#include <Bnd_BoundSortBox.hxx>

#include <IntCurveSurface_IntersectionPoint.hxx>
#include <gp_Lin.hxx>
#include <TopoDS_Face.hxx>
#include <TopAbs.hxx>


#include <IntCurveSurface_HInter.hxx>
#include <BRepAdaptor_HSurface.hxx>
#include <Geom_Line.hxx>
#include <gp_Pnt2d.hxx>
#include <BRepClass_FaceClassifier.hxx>

#include <GeomAdaptor_Curve.hxx>

#include <GeomAdaptor_HCurve.hxx>
#include <BRepAdaptor_HSurface.hxx>



#include <Adaptor3d_HSurfaceTool.hxx>
#include <IntCurveSurface_TheHCurveTool.hxx>
#include <Adaptor3d_HCurve.hxx>
#include <Bnd_Box.hxx>
#include <Intf_Tool.hxx>
#include <IntCurveSurface_ThePolyhedronOfHInter.hxx>
#include <IntCurveSurface_ThePolygonOfHInter.hxx>
#include <IntCurveSurface_SequenceOfPnt.hxx>



//=======================================================================
//function : SurfaceType
//purpose  : 
//=======================================================================
GeomAbs_SurfaceType IntCurvesFace_Intersector::SurfaceType() const 
{ 
  return(Adaptor3d_HSurfaceTool::GetType(Hsurface));
}
//=======================================================================
//function : IntCurvesFace_Intersector
//purpose  : 
//=======================================================================
IntCurvesFace_Intersector::IntCurvesFace_Intersector(const TopoDS_Face& Face,
						     const Standard_Real aTol)
: 
  Tol(aTol),
  done(Standard_False),
  nbpnt(0),
  PtrOnPolyhedron(NULL),
  PtrOnBndBounding(NULL)
{ 
  BRepAdaptor_Surface surface;
  face = Face;
  surface.Initialize(Face,Standard_True);
  Hsurface = new BRepAdaptor_HSurface(surface);
  myTopolTool = new BRepTopAdaptor_TopolTool(Hsurface);
  
  GeomAbs_SurfaceType SurfaceType = Adaptor3d_HSurfaceTool::GetType(Hsurface);
  if(   (SurfaceType != GeomAbs_Plane) 
     && (SurfaceType != GeomAbs_Cylinder)
     && (SurfaceType != GeomAbs_Cone) 
     && (SurfaceType != GeomAbs_Sphere)
     && (SurfaceType != GeomAbs_Torus)) {
    Standard_Integer nbsu,nbsv;
    Standard_Real U0,V0,U1,V1;
    U0 = Hsurface->FirstUParameter();
    U1 = Hsurface->LastUParameter();
    V0 = Hsurface->FirstVParameter();
    V1 = Hsurface->LastVParameter();

    Standard_Real aURes = Hsurface->UResolution(1.0);
    Standard_Real aVRes = Hsurface->VResolution(1.0);

    // Checking correlation between number of samples and length of the face along each axis
    const Standard_Real aTresh = 100.0;
    const Standard_Integer aMinSamples = 10;
    const Standard_Integer aMaxSamples = 40;
    const Standard_Integer aMaxSamples2 = aMaxSamples * aMaxSamples;
    Standard_Real dU = (U1 - U0) / aURes;
    Standard_Real dV = (V1 - V0) / aVRes;
    nbsu = myTopolTool->NbSamplesU();
    nbsv = myTopolTool->NbSamplesV();
    if (nbsu > aMaxSamples) nbsu = aMaxSamples;
    if (nbsv > aMaxSamples) nbsv = aMaxSamples;

    if (Max(dU, dV) > Min(dU, dV) * aTresh)
    {
      nbsu = (Standard_Integer)(Sqrt(dU / dV) * aMaxSamples);
      if (nbsu < aMinSamples) nbsu = aMinSamples;
      nbsv = aMaxSamples2 / nbsu;
      if (nbsv < aMinSamples)
      {
        nbsv = aMinSamples;
        nbsu = aMaxSamples2 / aMinSamples;
      }
    }
    PtrOnPolyhedron = (IntCurveSurface_ThePolyhedronOfHInter *)
        new IntCurveSurface_ThePolyhedronOfHInter(Hsurface,nbsu,nbsv,U0,V0,U1,V1);
  }
}
//=======================================================================
//function : InternalCall
//purpose  : 
//=======================================================================
void IntCurvesFace_Intersector::InternalCall(const IntCurveSurface_HInter &HICS,
					     const Standard_Real parinf,
					     const Standard_Real parsup) 
{
  if(HICS.IsDone()) {
    for(Standard_Integer index=HICS.NbPoints(); index>=1; index--) {  
      const IntCurveSurface_IntersectionPoint& HICSPointindex = HICS.Point(index);
      gp_Pnt2d Puv(HICSPointindex.U(),HICSPointindex.V());
      
      TopAbs_State currentstate = myTopolTool->Classify(Puv,Tol);
      if(currentstate==TopAbs_IN || currentstate==TopAbs_ON) { 
	Standard_Real HICSW = HICSPointindex.W();
	if(HICSW >= parinf && HICSW <= parsup ) { 
	  Standard_Real U          = HICSPointindex.U();
	  Standard_Real V          = HICSPointindex.V();
	  Standard_Real W          = HICSW; 
	  IntCurveSurface_TransitionOnCurve transition = HICSPointindex.Transition();
	  gp_Pnt pnt        = HICSPointindex.Pnt();
	  //	  state      = currentstate;
	  //  Modified by skv - Wed Sep  3 16:14:10 2003 OCC578 Begin
	  Standard_Integer anIntState = (currentstate == TopAbs_IN) ? 0 : 1;
	  //  Modified by skv - Wed Sep  3 16:14:11 2003 OCC578 End

	  if(transition != IntCurveSurface_Tangent && face.Orientation()==TopAbs_REVERSED) { 
	    if(transition == IntCurveSurface_In) 
	      transition = IntCurveSurface_Out;
	    else 
	      transition = IntCurveSurface_In;
	  }
	  //----- Insertion du point 
	  if(nbpnt==0) { 
	    IntCurveSurface_IntersectionPoint PPP(pnt,U,V,W,transition);
	    SeqPnt.Append(PPP);
	    //  Modified by skv - Wed Sep  3 16:14:10 2003 OCC578 Begin
	    mySeqState.Append(anIntState);
	    //  Modified by skv - Wed Sep  3 16:14:11 2003 OCC578 End
	  }
	  else { 
	    Standard_Integer i = 1;
	    Standard_Integer b = nbpnt+1;                    
	    while(i<=nbpnt) {
	      const IntCurveSurface_IntersectionPoint& Pnti=SeqPnt.Value(i);
	      Standard_Real wi = Pnti.W();
	      if(wi >= W) { b=i; i=nbpnt; }
	      i++;
	    }
	    IntCurveSurface_IntersectionPoint PPP(pnt,U,V,W,transition);
	    //  Modified by skv - Wed Sep  3 16:14:10 2003 OCC578 Begin
// 	    if(b>nbpnt)          { SeqPnt.Append(PPP); } 
// 	    else if(b>0)             { SeqPnt.InsertBefore(b,PPP); } 
	    if(b>nbpnt) {
	      SeqPnt.Append(PPP);
	      mySeqState.Append(anIntState);
	    } else if(b>0) {
	      SeqPnt.InsertBefore(b,PPP);
	      mySeqState.InsertBefore(b, anIntState);
	    }
	    //  Modified by skv - Wed Sep  3 16:14:11 2003 OCC578 End
	  }

 
	  nbpnt++;
	} 
      } //-- classifier state is IN or ON
    } //-- Loop on Intersection points.
  } //-- HICS.IsDone()
}
//=======================================================================
//function : Perform
//purpose  : 
//=======================================================================
void IntCurvesFace_Intersector::Perform(const gp_Lin& L,
					const Standard_Real ParMin,
					const Standard_Real ParMax)
{ 
  done = Standard_True;
  SeqPnt.Clear();
  mySeqState.Clear();
  nbpnt = 0;
  
  IntCurveSurface_HInter HICS; 
  Handle(Geom_Line) geomline = new Geom_Line(L);
  GeomAdaptor_Curve LL(geomline);
  Handle(GeomAdaptor_HCurve) HLL  = new GeomAdaptor_HCurve(LL);
  Standard_Real parinf=ParMin;
  Standard_Real parsup=ParMax;
  //
  if(PtrOnPolyhedron == NULL) { 
    HICS.Perform(HLL,Hsurface);
  }
  else { 
    Intf_Tool bndTool;
    Bnd_Box   boxLine;
    bndTool.LinBox
      (L,
       ((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron)->Bounding(),
       boxLine);
    if(bndTool.NbSegments() == 0) 
      return;
    for(Standard_Integer nbseg=1; nbseg<= bndTool.NbSegments(); nbseg++) { 
      Standard_Real pinf = bndTool.BeginParam(nbseg);
      Standard_Real psup = bndTool.EndParam(nbseg);
      Standard_Real pppp = 0.05*(psup-pinf);
      pinf-=pppp;
      psup+=pppp;
      if((psup - pinf)<1e-10) { pinf-=1e-10; psup+=1e-10; } 
      if(nbseg==1) { parinf=pinf; parsup=psup; }
      else { 
	if(parinf>pinf) parinf = pinf;
	if(parsup<psup) parsup = psup;
      }
    }
    if(parinf>ParMax) { return; } 
    if(parsup<ParMin) { return; }
    if(parinf<ParMin) parinf=ParMin;
    if(parsup>ParMax) parsup=ParMax;
    if(parinf>(parsup-1e-9)) return; 
    IntCurveSurface_ThePolygonOfHInter polygon(HLL,
					       parinf,
					       parsup,
					       2);
#if OPTIMISATION
    if(PtrOnBndBounding==NULL) { 
      PtrOnBndBounding = (Bnd_BoundSortBox *) new Bnd_BoundSortBox();
      IntCurveSurface_ThePolyhedronOfHInter *thePolyh=
	(IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron;
      ((Bnd_BoundSortBox *)(PtrOnBndBounding))->
	Initialize(IntCurveSurface_ThePolyhedronToolOfHInter::Bounding(*thePolyh),
		   IntCurveSurface_ThePolyhedronToolOfHInter::ComponentsBounding(*thePolyh));
    }
    HICS.Perform(HLL,
		 polygon,
		 Hsurface,
		 *((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron),
		 *((Bnd_BoundSortBox *)PtrOnBndBounding));
#else
    HICS.Perform(HLL,
		 polygon,
		 Hsurface,
		 *((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron));
#endif
  }
  
  InternalCall(HICS,parinf,parsup);
}
//=======================================================================
//function : Perform
//purpose  : 
//=======================================================================
void IntCurvesFace_Intersector::Perform(const Handle(Adaptor3d_HCurve)& HCu,
					const Standard_Real ParMin,
					const Standard_Real ParMax) 
{ 
  done = Standard_True;
  SeqPnt.Clear();
  //  Modified by skv - Wed Sep  3 16:14:10 2003 OCC578 Begin
  mySeqState.Clear();
  //  Modified by skv - Wed Sep  3 16:14:11 2003 OCC578 End
  nbpnt = 0;
  IntCurveSurface_HInter            HICS; 
  
  //-- 
  Standard_Real parinf=ParMin;
  Standard_Real parsup=ParMax;

  if(PtrOnPolyhedron == NULL) { 
    HICS.Perform(HCu,Hsurface);
  }
  else { 
    parinf = IntCurveSurface_TheHCurveTool::FirstParameter(HCu);
    parsup = IntCurveSurface_TheHCurveTool::LastParameter(HCu);
    if(parinf<ParMin) parinf = ParMin;
    if(parsup>ParMax) parsup = ParMax;
    if(parinf>(parsup-1e-9)) return; 
    Standard_Integer nbs;
    nbs = IntCurveSurface_TheHCurveTool::NbSamples(HCu,parinf,parsup);
    
    IntCurveSurface_ThePolygonOfHInter polygon(HCu,
					       parinf,
					       parsup,
					       nbs);
#if OPTIMISATION
    if(PtrOnBndBounding==NULL) { 
      PtrOnBndBounding = (Bnd_BoundSortBox *) new Bnd_BoundSortBox();
      IntCurveSurface_ThePolyhedronOfHInter *thePolyh=(IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron;
      ((Bnd_BoundSortBox *)(PtrOnBndBounding))->Initialize(IntCurveSurface_ThePolyhedronToolOfHInter::Bounding(*thePolyh),
							   IntCurveSurface_ThePolyhedronToolOfHInter::ComponentsBounding(*thePolyh));
    }
    HICS.Perform(HCu,
		 polygon,
		 Hsurface,
		 *((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron),
		 *((Bnd_BoundSortBox *)PtrOnBndBounding));
#else
    HICS.Perform(HCu,
		 polygon,
		 Hsurface,
		 *((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron));
#endif
  }
  InternalCall(HICS,parinf,parsup);
}

//============================================================================
Bnd_Box IntCurvesFace_Intersector::Bounding() const {
  if(PtrOnPolyhedron !=NULL) {
    return(((IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron)->Bounding());
  }
  else { 
    Bnd_Box B;
    return(B);
  }
}
TopAbs_State IntCurvesFace_Intersector::ClassifyUVPoint(const gp_Pnt2d& Puv) const { 
  TopAbs_State state = myTopolTool->Classify(Puv,1e-7);
  return(state);
}
//============================================================================
void IntCurvesFace_Intersector::Destroy() { 
  if(PtrOnPolyhedron !=NULL) { 
    delete (IntCurveSurface_ThePolyhedronOfHInter *)PtrOnPolyhedron;
    PtrOnPolyhedron = NULL;
  }
  if(PtrOnBndBounding !=NULL) { 
    delete (Bnd_BoundSortBox *)PtrOnBndBounding;
    PtrOnBndBounding=NULL;
  }
}



