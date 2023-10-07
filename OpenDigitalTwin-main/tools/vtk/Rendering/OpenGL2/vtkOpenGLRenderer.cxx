/*=========================================================================

Program:   Visualization Toolkit
Module:    vtkOpenGLRenderer.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkOpenGLRenderer.h"

#include "vtkOpenGLHelper.h"

#include "vtkCellArray.h"
#include "vtkDepthPeelingPass.h"
#include "vtkDualDepthPeelingPass.h"
#include "vtkFloatArray.h"
#include "vtkHardwareSelector.h"
#include "vtkHiddenLineRemovalPass.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkOpenGLCamera.h"
#include "vtkOpenGLError.h"
#include "vtkOpenGLFXAAFilter.h"
#include "vtkOpenGLRenderWindow.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkRenderPass.h"
#include "vtkRenderState.h"
#include "vtkRenderTimerLog.h"
#include "vtkShadowMapBakerPass.h"
#include "vtkShadowMapPass.h"
#include "vtkTexture.h"
#include "vtkTextureObject.h"
#include "vtkTexturedActor2D.h"
#include "vtkTimerLog.h"
#include "vtkTranslucentPass.h"
#include "vtkTrivialProducer.h"
#include "vtkUnsignedCharArray.h"
#include "vtkVolumetricPass.h"

#include <vtksys/RegularExpression.hxx>

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <list>
#include <sstream>
#include <string>

class vtkGLPickInfo
{
public:
  unsigned int PickedId;
  unsigned int NumPicked;
  bool PerformedHardwarePick;
  std::map<unsigned int,float> PickValues;
};

vtkStandardNewMacro(vtkOpenGLRenderer);

vtkOpenGLRenderer::vtkOpenGLRenderer()
{
  this->PickInfo = new vtkGLPickInfo;
  this->PickInfo->PickedId = 0;
  this->PickInfo->NumPicked = 0;
  this->PickedZ = 0;

  this->FXAAFilter = nullptr;
  this->DepthPeelingPass = nullptr;
  this->ShadowMapPass = nullptr;
  this->DepthPeelingHigherLayer=0;

  this->BackgroundTexture = nullptr;
  this->HaveApplePrimitiveIdBugValue = false;
  this->HaveApplePrimitiveIdBugChecked = false;
}

// Ask lights to load themselves into graphics pipeline.
int vtkOpenGLRenderer::UpdateLights ()
{
  vtkOpenGLClearErrorMacro();

  VTK_SCOPED_RENDER_EVENT("vtkOpenGLRenderer::UpdateLights",
                          this->GetRenderWindow()->GetRenderTimer());

  vtkLight *light;
  float status;
  int count = 0;

  vtkCollectionSimpleIterator sit;
  for(this->Lights->InitTraversal(sit);
      (light = this->Lights->GetNextLight(sit)); )
  {
    status = light->GetSwitch();
    if (status > 0.0)
    {
      count++;
    }
  }

  if( !count )
  {
    vtkDebugMacro(<<"No lights are on, creating one.");
    this->CreateLight();
  }

  for(this->Lights->InitTraversal(sit);
      (light = this->Lights->GetNextLight(sit)); )
  {
    status = light->GetSwitch();

    // if the light is on then define it and bind it.
    if (status > 0.0)
    {
      light->Render(this,0);
    }
  }

  vtkOpenGLCheckErrorMacro("failed after UpdateLights");

  return count;
}

// ----------------------------------------------------------------------------
// Description:
// Is rendering at translucent geometry stage using depth peeling and
// rendering a layer other than the first one? (Boolean value)
// If so, the uniform variables UseTexture and Texture can be set.
// (Used by vtkOpenGLProperty or vtkOpenGLTexture)
int vtkOpenGLRenderer::GetDepthPeelingHigherLayer()
{
  return this->DepthPeelingHigherLayer;
}

// ----------------------------------------------------------------------------
// Concrete open gl render method.
void vtkOpenGLRenderer::DeviceRender(void)
{
  vtkTimerLog::MarkStartEvent("OpenGL Dev Render");

  if(this->Pass!=nullptr)
  {
    vtkRenderState s(this);
    s.SetPropArrayAndCount(this->PropArray, this->PropArrayCount);
    s.SetFrameBuffer(nullptr);
    this->Pass->Render(&s);
  }
  else
  {
    // Do not remove this MakeCurrent! Due to Start / End methods on
    // some objects which get executed during a pipeline update,
    // other windows might get rendered since the last time
    // a MakeCurrent was called.
    this->RenderWindow->MakeCurrent();
    vtkOpenGLClearErrorMacro();

    this->UpdateCamera();
    this->UpdateLightGeometry();
    this->UpdateLights();
    this->UpdateGeometry();

    vtkOpenGLCheckErrorMacro("failed after DeviceRender");
  }

  vtkTimerLog::MarkEndEvent("OpenGL Dev Render");
}

// Ask actors to render themselves. As a side effect will cause
// visualization network to update.
int vtkOpenGLRenderer::UpdateGeometry()
{
  vtkRenderTimerLog *timer = this->GetRenderWindow()->GetRenderTimer();
  VTK_SCOPED_RENDER_EVENT("vtkOpenGLRenderer::UpdateGeometry", timer);

  int        i;

  this->NumberOfPropsRendered = 0;

  if ( this->PropArrayCount == 0 )
  {
    return 0;
  }

  if (this->Selector)
  {
    VTK_SCOPED_RENDER_EVENT2("Selection", timer, selectionEvent);

    // When selector is present, we are performing a selection,
    // so do the selection rendering pass instead of the normal passes.
    // Delegate the rendering of the props to the selector itself.
    this->NumberOfPropsRendered = this->Selector->Render(this,
      this->PropArray, this->PropArrayCount);
    this->RenderTime.Modified();
    vtkDebugMacro("Rendered " << this->NumberOfPropsRendered << " actors" );
    return this->NumberOfPropsRendered;
  }

  // if we are suing shadows then let the renderpasses handle it
  // for opaque and translucent
  int hasTranslucentPolygonalGeometry = 0;
  if (this->UseShadows)
  {
    VTK_SCOPED_RENDER_EVENT2("Shadows", timer, shadowsEvent);

    if (!this->ShadowMapPass)
    {
      this->ShadowMapPass = vtkShadowMapPass::New();
    }
    vtkRenderState s(this);
    s.SetPropArrayAndCount(this->PropArray, this->PropArrayCount);
    //s.SetFrameBuffer(0);
    this->ShadowMapPass->GetShadowMapBakerPass()->Render(&s);
    this->ShadowMapPass->Render(&s);
  }
  else
  {
    // Opaque geometry first:
    timer->MarkStartEvent("Opaque Geometry");
    this->DeviceRenderOpaqueGeometry();
    timer->MarkEndEvent();

    // do the render library specific stuff about translucent polygonal geometry.
    // As it can be expensive, do a quick check if we can skip this step
    for ( i = 0; !hasTranslucentPolygonalGeometry && i < this->PropArrayCount;
          i++ )
    {
      hasTranslucentPolygonalGeometry=
        this->PropArray[i]->HasTranslucentPolygonalGeometry();
    }
    if(hasTranslucentPolygonalGeometry)
    {
      timer->MarkStartEvent("Translucent Geometry");
      this->DeviceRenderTranslucentPolygonalGeometry();
      timer->MarkEndEvent();
    }
  }

  // Apply FXAA before volumes and overlays. Volumes don't need AA, and overlays
  // are usually things like text, which are already antialiased.
  if (this->UseFXAA)
  {
    timer->MarkStartEvent("FXAA");
    if (!this->FXAAFilter)
    {
      this->FXAAFilter = vtkOpenGLFXAAFilter::New();
    }
    if (this->FXAAOptions)
    {
      this->FXAAFilter->UpdateConfiguration(this->FXAAOptions);
    }

    this->FXAAFilter->Execute(this);
    timer->MarkEndEvent();
  }

  // loop through props and give them a chance to
  // render themselves as volumetric geometry.
  if (hasTranslucentPolygonalGeometry == 0 || !this->UseDepthPeelingForVolumes)
  {
    timer->MarkStartEvent("Volumes");
    for ( i = 0; i < this->PropArrayCount; i++ )
    {
      this->NumberOfPropsRendered +=
        this->PropArray[i]->RenderVolumetricGeometry(this);
    }
    timer->MarkEndEvent();
  }

  // loop through props and give them a chance to
  // render themselves as an overlay (or underlay)
  timer->MarkStartEvent("Overlay");
  for ( i = 0; i < this->PropArrayCount; i++ )
  {
    this->NumberOfPropsRendered +=
      this->PropArray[i]->RenderOverlay(this);
  }
  timer->MarkEndEvent();

  this->RenderTime.Modified();

  vtkDebugMacro( << "Rendered " <<
                    this->NumberOfPropsRendered << " actors" );

  return  this->NumberOfPropsRendered;
}

// ----------------------------------------------------------------------------
void vtkOpenGLRenderer::DeviceRenderOpaqueGeometry()
{
  // Do we need hidden line removal?
  bool useHLR =
      this->UseHiddenLineRemoval &&
      vtkHiddenLineRemovalPass::WireframePropsExist(this->PropArray,
                                                    this->PropArrayCount);

  if (useHLR)
  {
    vtkNew<vtkHiddenLineRemovalPass> hlrPass;
    vtkRenderState s(this);
    s.SetPropArrayAndCount(this->PropArray, this->PropArrayCount);
    s.SetFrameBuffer(nullptr);
    hlrPass->Render(&s);
    this->NumberOfPropsRendered += hlrPass->GetNumberOfRenderedProps();
  }
  else
  {
    this->Superclass::DeviceRenderOpaqueGeometry();
  }
}

// ----------------------------------------------------------------------------
// Description:
// Render translucent polygonal geometry. Default implementation just call
// UpdateTranslucentPolygonalGeometry().
// Subclasses of vtkRenderer that can deal with depth peeling must
// override this method.
void vtkOpenGLRenderer::DeviceRenderTranslucentPolygonalGeometry()
{
  vtkOpenGLClearErrorMacro();

  vtkOpenGLRenderWindow *context
    = vtkOpenGLRenderWindow::SafeDownCast(this->RenderWindow);

  if(this->UseDepthPeeling && !context)
  {
    vtkErrorMacro("OpenGL render window is required.")
    return;
  }

  if(!this->UseDepthPeeling)
  {
    // just alpha blending
    this->UpdateTranslucentPolygonalGeometry();
  }
  else   // depth peeling.
  {
#if GL_ES_VERSION_3_0 == 1
    vtkErrorMacro("Built in Dual Depth Peeling is not supported on ES3. "
      "Please see TestFramebufferPass.cxx for an example that should work "
      "on OpenGL ES 3.");
    this->UpdateTranslucentPolygonalGeometry();
#else
    if (!this->DepthPeelingPass)
    {
      if (this->IsDualDepthPeelingSupported())
      {
        vtkDebugMacro("Using dual depth peeling.");
        vtkDualDepthPeelingPass *ddpp = vtkDualDepthPeelingPass::New();
        this->DepthPeelingPass = ddpp;
      }
      else
      {
        vtkDebugMacro("Using standard depth peeling (dual depth peeling not "
                      "supported by the graphics card/driver).");
        this->DepthPeelingPass = vtkDepthPeelingPass::New();
      }
      vtkTranslucentPass *tp = vtkTranslucentPass::New();
      this->DepthPeelingPass->SetTranslucentPass(tp);
      tp->Delete();
    }

    if (this->UseDepthPeelingForVolumes)
    {
      vtkDualDepthPeelingPass *ddpp = vtkDualDepthPeelingPass::SafeDownCast(
            this->DepthPeelingPass);
      if (!ddpp)
      {
        vtkWarningMacro("UseDepthPeelingForVolumes requested, but unsupported "
                        "since DualDepthPeeling is not available.");
        this->UseDepthPeelingForVolumes = false;
      }
      else if (!ddpp->GetVolumetricPass())
      {
        vtkVolumetricPass *vp = vtkVolumetricPass::New();
        ddpp->SetVolumetricPass(vp);
        vp->Delete();
      }
    }
    else
    {
      vtkDualDepthPeelingPass *ddpp = vtkDualDepthPeelingPass::SafeDownCast(
            this->DepthPeelingPass);
      if (ddpp)
      {
        ddpp->SetVolumetricPass(nullptr);
      }
    }

    this->DepthPeelingPass->SetMaximumNumberOfPeels(this->MaximumNumberOfPeels);
    this->DepthPeelingPass->SetOcclusionRatio(this->OcclusionRatio);
    vtkRenderState s(this);
    s.SetPropArrayAndCount(this->PropArray, this->PropArrayCount);
    s.SetFrameBuffer(nullptr);
    this->LastRenderingUsedDepthPeeling=1;
    this->DepthPeelingPass->Render(&s);
    this->NumberOfPropsRendered += this->DepthPeelingPass->GetNumberOfRenderedProps();
#endif
  }

  vtkOpenGLCheckErrorMacro("failed after DeviceRenderTranslucentPolygonalGeometry");
}


// ----------------------------------------------------------------------------
void vtkOpenGLRenderer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "PickedId" << this->PickInfo->PickedId<< "\n";
  os << indent << "NumPicked" << this->PickInfo->NumPicked<< "\n";
  os << indent << "PickedZ " << this->PickedZ << "\n";
}


void vtkOpenGLRenderer::Clear(void)
{
  vtkOpenGLClearErrorMacro();

  GLbitfield  clear_mask = 0;

  if (! this->Transparent())
  {
    if (this->IsPicking)
    {
      glClearColor(0.0,0.0,0.0,0.0);
    }
    else
    {
      glClearColor(static_cast<GLclampf>(this->Background[0]),
        static_cast<GLclampf>(this->Background[1]), static_cast<GLclampf>(this->Background[2]),
        static_cast<GLclampf>(this->BackgroundAlpha));
    }
    clear_mask |= GL_COLOR_BUFFER_BIT;
  }

  if (!this->GetPreserveDepthBuffer())
  {
#if GL_ES_VERSION_3_0 == 1
    glClearDepthf(static_cast<GLclampf>(1.0));
#else
    glClearDepth(static_cast<GLclampf>(1.0));
#endif
    clear_mask |= GL_DEPTH_BUFFER_BIT;
    glDepthMask(GL_TRUE);
  }

  vtkDebugMacro(<< "glClear\n");
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glClear(clear_mask);

  // If gradient background is turned on, draw it now.
  if (!this->IsPicking && !this->Transparent() &&
      (this->GradientBackground || this->TexturedBackground))
  {
    int size[2];
    size[0] = this->GetSize()[0];
    size[1] = this->GetSize()[1];

    double tile_viewport[4];
    this->GetRenderWindow()->GetTileViewport(tile_viewport);

    vtkNew<vtkTexturedActor2D> actor;
    vtkNew<vtkPolyDataMapper2D> mapper;
    vtkNew<vtkPolyData> polydata;
    vtkNew<vtkPoints> points;
    points->SetNumberOfPoints(4);
    points->SetPoint(0, 0, 0, 0);
    points->SetPoint(1, size[0], 0, 0);
    points->SetPoint(2, size[0], size[1], 0);
    points->SetPoint(3, 0, size[1], 0);
    polydata->SetPoints(points);

    vtkNew<vtkCellArray> tris;
    tris->InsertNextCell(3);
    tris->InsertCellPoint(0);
    tris->InsertCellPoint(1);
    tris->InsertCellPoint(2);
    tris->InsertNextCell(3);
    tris->InsertCellPoint(0);
    tris->InsertCellPoint(2);
    tris->InsertCellPoint(3);
    polydata->SetPolys(tris);

    vtkNew<vtkTrivialProducer> prod;
    prod->SetOutput(polydata);

    // Set some properties.
    mapper->SetInputConnection(prod->GetOutputPort());
    actor->SetMapper(mapper);

    if(this->TexturedBackground && this->BackgroundTexture)
    {
      this->BackgroundTexture->InterpolateOn();
      actor->SetTexture(this->BackgroundTexture);

      vtkNew<vtkFloatArray> tcoords;
      float tmp[2];
      tmp[0] = 0;
      tmp[1] = 0;
      tcoords->SetNumberOfComponents(2);
      tcoords->SetNumberOfTuples(4);
      tcoords->SetTuple(0,tmp);
      tmp[0] = 1.0;
      tcoords->SetTuple(1,tmp);
      tmp[1] = 1.0;
      tcoords->SetTuple(2,tmp);
      tmp[0] = 0.0;
      tcoords->SetTuple(3,tmp);
      polydata->GetPointData()->SetTCoords(tcoords);
    }
    else // gradient
    {
      vtkNew<vtkUnsignedCharArray> colors;
      float tmp[4];
      tmp[0] = this->Background[0]*255;
      tmp[1] = this->Background[1]*255;
      tmp[2] = this->Background[2]*255;
      tmp[3] = 255;
      colors->SetNumberOfComponents(4);
      colors->SetNumberOfTuples(4);
      colors->SetTuple(0,tmp);
      colors->SetTuple(1,tmp);
      tmp[0] = this->Background2[0]*255;
      tmp[1] = this->Background2[1]*255;
      tmp[2] = this->Background2[2]*255;
      colors->SetTuple(2,tmp);
      colors->SetTuple(3,tmp);
      polydata->GetPointData()->SetScalars(colors);
    }

    glDisable(GL_DEPTH_TEST);
    actor->RenderOverlay(this);
  }

  glEnable(GL_DEPTH_TEST);

  vtkOpenGLCheckErrorMacro("failed after Clear");
}

void vtkOpenGLRenderer::StartPick(unsigned int vtkNotUsed(pickFromSize))
{
  vtkOpenGLClearErrorMacro();

  /*
  int size[2];
  size[0] = this->GetSize()[0];
  size[1] = this->GetSize()[1];

  // Create the FBO
  glGenFramebuffers(1, &this->PickInfo->PickingFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->PickInfo->PickingFBO);

  // Create the texture object for the primitive information buffer
  glGenTextures(1, &this->PickInfo->PickingTexture);
  glBindTexture(GL_TEXTURE_2D, this->PickInfo->PickingTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, size[0], size[1],
              0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
              this->PickInfo->PickingTexture, 0);

  // Create the texture object for the depth buffer
  glGenTextures(1, &this->PickInfo->DepthTexture);
  glBindTexture(GL_TEXTURE_2D, this->PickInfo->DepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size[0], size[1],
              0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
              this->PickInfo->DepthTexture, 0);

  // Disable reading to avoid problems with older GPUs
  glReadBuffer(GL_NONE);

  // Verify that the FBO is correct
  GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
    printf("FB error, status: 0x%x\n", Status);
    return;
    }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->PickInfo->PickingFBO);
  */

  // Do not remove this MakeCurrent! Due to Start / End methods on
  // some objects which get executed during a pipeline update,
  // other windows might get rendered since the last time
  // a MakeCurrent was called.
  this->RenderWindow->MakeCurrent();
  this->RenderWindow->IsPickingOn();
  this->IsPicking = 1;
  this->PickInfo->PerformedHardwarePick = false;
  this->PickInfo->PickValues.clear();
  this->PickInfo->NumPicked = 0;
  this->PickInfo->PickedId = 0;

  this->UpdateCamera();
  this->Clear();

  vtkOpenGLCheckErrorMacro("failed after StartPick");
}

void vtkOpenGLRenderer::ReleaseGraphicsResources(vtkWindow *w)
{
  if (w && this->Pass)
  {
    this->Pass->ReleaseGraphicsResources(w);
  }
  if (this->FXAAFilter)
  {
    this->FXAAFilter->ReleaseGraphicsResources();
  }
  if (w && this->DepthPeelingPass)
  {
    this->DepthPeelingPass->ReleaseGraphicsResources(w);
  }
  if (w && this->ShadowMapPass)
  {
    this->ShadowMapPass->ReleaseGraphicsResources(w);
  }
  this->Superclass::ReleaseGraphicsResources(w);
}

void vtkOpenGLRenderer::UpdatePickId()
{
  this->CurrentPickId++;
}


void vtkOpenGLRenderer::DevicePickRender()
{
  // Do not remove this MakeCurrent! Due to Start / End methods on
  // some objects which get executed during a pipeline update,
  // other windows might get rendered since the last time
  // a MakeCurrent was called.
  this->RenderWindow->MakeCurrent();
  vtkOpenGLClearErrorMacro();

#if GL_ES_VERSION_3_0 != 1
  bool msaaWasEnabled = false;
  if (this->RenderWindow->GetMultiSamples() > 0 && glIsEnabled(GL_MULTISAMPLE))
  {
    glDisable(GL_MULTISAMPLE);
    msaaWasEnabled = true;
  }
#endif

  this->UpdateCamera();
  this->UpdateLightGeometry();
  this->UpdateLights();

  this->PickGeometry();

  this->PickInfo->PerformedHardwarePick = true;

#if GL_ES_VERSION_3_0 != 1
  if (msaaWasEnabled)
  {
    glEnable(GL_MULTISAMPLE);
  }
#endif

  vtkOpenGLCheckErrorMacro("failed after DevicePickRender");
}


void vtkOpenGLRenderer::DonePick()
{
  if (this->PickInfo->PerformedHardwarePick)
  {
    unsigned char *pixBuffer = this->GetRenderWindow()->GetPixelData(
      this->PickX1, this->PickY1, this->PickX2, this->PickY2, 0);
  //    (this->GetRenderWindow()->GetSwapBuffers() == 1) ? 0 : 1);

    // for debugging save out the image
    // FILE * pFile;
    // pFile = fopen ("myfile.ppm", "wb");
    // fwrite (pixBuffer , sizeof(unsigned char), 3*((int)this->PickY2-(int)this->PickY1+1)*((int)this->PickX2-(int)this->PickX1+1), pFile);
    // fclose (pFile);

    float *depthBuffer = this->GetRenderWindow()->GetZbufferData(
      this->PickX1, this->PickY1, this->PickX2, this->PickY2);

    // read the color and z buffer values for the region
    // to see what hits we have
    this->PickInfo->PickValues.clear();
    unsigned char *pb = pixBuffer;
    float *dbPtr = depthBuffer;
    for (int y = this->PickY1; y <= this->PickY2; y++)
    {
      for (int x = this->PickX1; x <= this->PickX2; x++)
      {
        unsigned char rgb[3];
        rgb[0] = *pb++;
        rgb[1] = *pb++;
        rgb[2] = *pb++;
        int val = 0;
        val |= rgb[2];
        val = val << 8;
        val |= rgb[1];
        val = val << 8;
        val |= rgb[0];
        if (val > 0)
        {
          if (this->PickInfo->PickValues.find(val) == this->PickInfo->PickValues.end())
          {
            this->PickInfo->PickValues.insert(std::pair<unsigned int,float>(val,*dbPtr));
          }
        }
        dbPtr++;
      }
    }

    delete [] pixBuffer;
    delete [] depthBuffer;

    this->PickInfo->NumPicked = (unsigned int)this->PickInfo->PickValues.size();

    this->PickInfo->PickedId = 0;
    std::map<unsigned int,float>::const_iterator dvItr =
      this->PickInfo->PickValues.begin();
    this->PickedZ = 1.0;
    for ( ; dvItr != this->PickInfo->PickValues.end(); ++dvItr)
    {
      if(dvItr->second < this->PickedZ)
      {
        this->PickedZ = dvItr->second;
        this->PickInfo->PickedId = dvItr->first - 1;
      }
    }
  }

  // Restore the default framebuffer
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  this->RenderWindow->IsPickingOff();
  this->IsPicking = 0;
}

double vtkOpenGLRenderer::GetPickedZ()
{
  return this->PickedZ;
}

unsigned int vtkOpenGLRenderer::GetPickedId()
{
  return static_cast<unsigned int>(this->PickInfo->PickedId);
}

vtkOpenGLRenderer::~vtkOpenGLRenderer()
{
  delete this->PickInfo;

  if(this->Pass != nullptr)
  {
    this->Pass->UnRegister(this);
    this->Pass = nullptr;
  }

  if (this->FXAAFilter)
  {
    this->FXAAFilter->Delete();
    this->FXAAFilter = nullptr;
  }

  if (this->ShadowMapPass)
  {
    this->ShadowMapPass->Delete();
    this->ShadowMapPass = nullptr;
  }

  if (this->DepthPeelingPass)
  {
    this->DepthPeelingPass->Delete();
    this->DepthPeelingPass = nullptr;
  }
}

bool vtkOpenGLRenderer::HaveApplePrimitiveIdBug()
{
  if (this->HaveApplePrimitiveIdBugChecked)
  {
    return this->HaveApplePrimitiveIdBugValue;
  }

#ifdef __APPLE__
  // Known working Apple+AMD systems:
  // OpenGL vendor string:  ATI Technologies Inc.
  // OpenGL version string:   4.1 ATI-1.38.3
  // OpenGL version string:   4.1 ATI-1.40.15
  // OpenGL renderer string:    AMD Radeon R9 M370X OpenGL Engine

  // OpenGL version string:   4.1 ATI-1.40.16
  // OpenGL renderer string:    AMD Radeon HD - FirePro D500 OpenGL Engine
  // OpenGL renderer string:    AMD Radeon HD 5770 OpenGL Engine
  // OpenGL renderer string:    AMD Radeon R9 M395 OpenGL Engine

  // OpenGL vendor string:  ATI Technologies Inc.
  // OpenGL renderer string:  ATI Radeon HD 5770 OpenGL Engine
  // OpenGL version string:  4.1 ATI-1.42.6

  // Known buggy Apple+AMD systems:
  // OpenGL vendor string:  ATI Technologies Inc.
  // OpenGL version string:   3.3 ATI-10.0.40
  // OpenGL renderer string:    ATI Radeon HD 2600 PRO OpenGL Engine

  // OpenGL vendor string:  ATI Technologies Inc.
  // OpenGL renderer string:  AMD Radeon HD - FirePro D300 OpenGL Engine
  // OpenGL version string:  4.1 ATI-1.24.39

  std::string vendor = (const char *)glGetString(GL_VENDOR);
  if (vendor.find("ATI") != std::string::npos ||
      vendor.find("AMD") != std::string::npos ||
      vendor.find("amd") != std::string::npos)
  {
    // assume we have the bug
    this->HaveApplePrimitiveIdBugValue = true;

    // but exclude systems we know do not have it
    std::string renderer = (const char *)glGetString(GL_RENDERER);
    std::string version = (const char *)glGetString(GL_VERSION);
    int minorVersion = 0;
    int patchVersion = 0;
    // try to extract some minor version numbers
    if (version.find("4.1 ATI-1.") == 0)
    {
      std::string minorVer = version.substr(strlen("4.1 ATI-1."),std::string::npos);
      if (minorVer.find('.') == 2)
      {
        minorVersion = atoi(minorVer.substr(0,2).c_str());
        patchVersion = atoi(minorVer.substr(3,std::string::npos).c_str());
      }
    }
    if (
        ((version.find("4.1 ATI-1.38.3") != std::string::npos ||
          version.find("4.1 ATI-1.40.15") != std::string::npos) &&
          (renderer.find("AMD Radeon R9 M370X OpenGL Engine") != std::string::npos)) ||
          // assume anything with 1.40.16 or later is good?
          minorVersion > 40 ||
          (minorVersion == 40 && patchVersion >= 16)
         )
    {
      this->HaveApplePrimitiveIdBugValue = false;
    }
  }
#else
  this->HaveApplePrimitiveIdBugValue = false;
#endif

  this->HaveApplePrimitiveIdBugChecked = true;
  return this->HaveApplePrimitiveIdBugValue;
}

//------------------------------------------------------------------------------
bool vtkOpenGLRenderer::HaveAppleQueryAllocationBug()
{
#ifndef __APPLE__ // Bug only applies to apple
  return false;
#else
  enum class QueryAllocStatus { NotChecked, Yes, No };
  static QueryAllocStatus hasBug = QueryAllocStatus::NotChecked;

  if (hasBug == QueryAllocStatus::NotChecked)
  {
    // We can restrict this to a specific version, etc, as we get more
    // information about the bug, but for now just disable query allocations on
    // all apple NVIDIA cards.
    std::string v = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    hasBug = (v.find("NVIDIA") != std::string::npos) ? QueryAllocStatus::Yes
                                                     : QueryAllocStatus::No;
  }

  return hasBug == QueryAllocStatus::Yes;
#endif
}

//------------------------------------------------------------------------------
bool vtkOpenGLRenderer::IsDualDepthPeelingSupported()
{
  vtkOpenGLRenderWindow *context
    = vtkOpenGLRenderWindow::SafeDownCast(this->RenderWindow);
  if (!context)
  {
    vtkDebugMacro("Cannot determine if dual depth peeling is support -- no "
                  "vtkRenderWindow set.");
    return false;
  }

  // Dual depth peeling requires:
  // - float textures (ARB_texture_float)
  // - RG textures (ARB_texture_rg)
  // - MAX blending (added in ES3).
  // requires that RG textures be color renderable (they are not in ES3)
#if GL_ES_VERSION_3_0 == 1
  // ES3 is not supported, see TestFramebufferPass.cxx for how to do it
  bool dualDepthPeelingSupported = false;
#else
  bool dualDepthPeelingSupported = context->GetContextSupportsOpenGL32() ||
      (GLEW_ARB_texture_float && GLEW_ARB_texture_rg);
#endif

  // There's a bug on current mesa master that prevents dual depth peeling
  // from functioning properly, something in the texture sampler is causing
  // all lookups to return NaN. See discussion on
  // https://bugs.freedesktop.org/show_bug.cgi?id=94955
  // This has been fixed in Mesa 17.2.
  const char *glVersionC =
      reinterpret_cast<const char *>(glGetString(GL_VERSION));
  std::string glVersion = std::string(glVersionC ? glVersionC : "");
  if (dualDepthPeelingSupported &&
      glVersion.find("Mesa") != std::string::npos)
  {
    bool mesaCompat = false;
    // The bug has been fixed with mesa 17.2.0. The version string is approx:
    // 3.3 (Core Profile) Mesa 17.2.0-devel (git-08cb8cf256)
    vtksys::RegularExpression re("Mesa ([0-9]+)\\.([0-9]+)\\.");
    if (re.find(glVersion))
    {
      int majorVersion;
      std::string majorStr = re.match(1);
      std::istringstream majorParse(majorStr);
      majorParse >> majorVersion;
      if (majorVersion > 17)
      {
        mesaCompat = true;
      }
      else if (majorVersion == 17)
      {
        int minorVersion;
        std::string minorStr = re.match(2);
        std::istringstream minorParse(minorStr);
        minorParse >> minorVersion;
        if (minorVersion >= 2)
        {
          mesaCompat = true;
        }
      }
    }

    if (!mesaCompat)
    {
      vtkDebugMacro("Disabling dual depth peeling -- mesa bug detected. "
                    "GL_VERSION = '" << glVersion << "'.");
      dualDepthPeelingSupported = false;
    }
  }

  // The old implemention can be forced by defining the environment var
  // "VTK_USE_LEGACY_DEPTH_PEELING":
  if (dualDepthPeelingSupported)
  {
    const char *forceLegacy = getenv("VTK_USE_LEGACY_DEPTH_PEELING");
    if (forceLegacy)
    {
      vtkDebugMacro("Disabling dual depth peeling -- "
                    "VTK_USE_LEGACY_DEPTH_PEELING defined in environment.");
      dualDepthPeelingSupported = false;
    }
  }

  return dualDepthPeelingSupported;
}

unsigned int vtkOpenGLRenderer::GetNumPickedIds()
{
  return static_cast<unsigned int>(this->PickInfo->NumPicked);
}

int vtkOpenGLRenderer::GetPickedIds(unsigned int atMost,
                                    unsigned int *callerBuffer)
{
  if (this->PickInfo->PickValues.empty())
  {
    return 0;
  }

  unsigned int max = (atMost < this->PickInfo->NumPicked) ? atMost : this->PickInfo->NumPicked;

  unsigned int k = 0;
  unsigned int *optr = callerBuffer;
  std::map<unsigned int,float>::const_iterator dvItr =
    this->PickInfo->PickValues.begin();
  this->PickedZ = 1.0;
  for ( ; dvItr != this->PickInfo->PickValues.end() && k < max; ++dvItr)
  {
    *optr = static_cast<unsigned int>(dvItr->first);
    optr++;
  }
  return k;
}
