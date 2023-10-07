// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <string.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Repeat_Button.H>
#include "GmshConfig.h"
#include "FlGui.h"
#include "projectionEditor.h"
#include "paletteWindow.h"
#include "fileDialogs.h"
#include "GModelIO_Fourier.h"
#include "MElement.h"
#include "drawContext.h"
#include "Options.h"
#include "StringUtils.h"
#include "fourierFace.h"
#include "GmshMessage.h"
#include "Context.h"
#include "OS.h"

#if defined(HAVE_FOURIER_MODEL)

#include "FM_BlendedPatch.h"
#include "FM_BlendOperator.h"
#include "FM_PlaneProjectionSurface.h"
#include "FM_TrapezoidProjectionSurface.h"
#include "FM_SphericalProjectionSurface.h"
#include "FM_ParaboloidProjectionSurface.h"
#include "FM_CylindricalProjectionSurface.h"
#include "FM_RevolvedParabolaProjectionSurface.h"
#include "FM_TranslatedParabolaProjectionSurface.h"

static fourierProjectionFace *createProjectionFaceFromName(const char *name)
{
  GModel *m = GModel::current();
  int tag = m->getNumFaces() + 1;
  fourierProjectionFace *f = 0;
  if(!strcmp(name, "plane"))
    f = new fourierProjectionFace(m, tag, new FM::PlaneProjectionSurface(tag));
  else if(!strcmp(name, "trapezoid"))
    f = new fourierProjectionFace(m, tag, new FM::TrapezoidProjectionSurface(tag));
  else if(!strcmp(name, "sphere"))
    f = new fourierProjectionFace(m, tag, new FM::SphericalProjectionSurface(tag));
  else if(!strcmp(name, "paraboloid"))
    f = new fourierProjectionFace(m, tag, new FM::ParaboloidProjectionSurface(tag));
  else if(!strcmp(name, "cylinder"))
    f = new fourierProjectionFace(m, tag, new FM::CylindricalProjectionSurface(tag));
  else if(!strcmp(name, "revolvedParabola"))
    f = new fourierProjectionFace(m, tag, new FM::RevolvedParabolaProjectionSurface(tag));
  else if(!strcmp(name, "translatedParabola"))
    f = new fourierProjectionFace(m, tag, new FM::TranslatedParabolaProjectionSurface(tag));
  else
    Msg::Error("Unknown projection face `%s'", name);
  if(f){
    f->setVisibility(false);
    m->add(f);
  }
  return f;
}

static void project_point(FM::ProjectionSurface *ps, double x, double y, double z,
                          std::vector<double> &u, std::vector<double> &v,
                          std::vector<double> &dist,
                          std::vector<std::complex<double> > &f)
{
  double uu, vv, p[3], n[3];
  ps->OrthoProjectionOnSurface(x, y, z, uu, vv);
  if(uu >= 0. && uu <= 1. && vv >= 0. && vv <= 1.){
    ps->F(uu, vv, p[0], p[1], p[2]);
    ps->GetUnitNormal(uu, vv, n[0], n[1], n[2]);
    double dx = x - p[0], dy = y - p[1], dz = z - p[2];
    u.push_back(uu);
    v.push_back(vv);
    dist.push_back(sqrt(dx * dx + dy * dy + dz * dz));
    f.push_back(dx * n[0] + dy * n[1] + dz * n[2]);
  }
}

static void getTangents(const SVector3 n, SVector3 &t1, SVector3 &t2, const double angle)
{
  SVector3 ex(0., 0., 0.);
  if(n[0] == 0.)
    ex[0] = 1.;
  else if(n[1] == 0.)
    ex[1] = 1.;
  else
    ex[2] = 1.;
  SVector3 a = crossprod(n, ex);
  a.normalize();
  SVector3 b = crossprod(n, a);
  b.normalize();
  double x = n[0], y = n[1], z = n[2];
  double c = cos(angle * M_PI / 180.), s = sin(angle * M_PI / 180.);
  double rot[3][3] =
    {{x * x * (1-c) + c    , x * y * (1-c) - z * s, x * z * (1-c) + y * s},
     {y * x * (1-c) + z * s, y * y * (1-c) + c    , y * z * (1-c) - x * s},
     {x * z * (1-c) - y * s, y * z * (1-c) + x * s, z * z * (1-c) + c    }};
  for(int i = 0; i < 3; i++){
    t1[i] = t2[i] = 0.;
    for(int j = 0; j < 3; j++){
      t1[i] += rot[i][j] * a[j];
      t2[i] += rot[i][j] * b[j];
    }
  }
}

static void update_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;

  // get all parameters from GUI and modify projection surface accordingly

  projection *p = e->getCurrentProjection();
  if(p){
    FM::ProjectionSurface *ps = (FM::ProjectionSurface*)p->face->getNativePtr();
    ps->SetOrigin(p->parameters[0]->value(),
                  p->parameters[1]->value(),
                  p->parameters[2]->value());
    SVector3 n(p->parameters[3]->value(),
               p->parameters[4]->value(),
               p->parameters[5]->value());
    if(!n.normalize()) n[2] = 1.;
    SVector3 t1, t2;
    getTangents(n, t1, t2, p->parameters[6]->value());
    ps->SetE0(n[0], n[1], n[2]);
    ps->SetE1(t1[0], t1[1], t1[2]);
    ps->SetE2(t2[0], t2[1], t2[2]);
    ps->SetScale(p->parameters[7]->value(),
                 p->parameters[8]->value(),
                 p->parameters[9]->value());
    for (int i = 0; i < ps->GetNumParameters(); i++)
      ps->SetParameter(i, p->parameters[i + 10]->value());

    p->face->buildSTLTriangulation();

    // project selected points and elements and update u,v display
    std::vector<double> u, v, dist;
    std::vector<std::complex<double> > f;
    std::vector<GEntity*> &ent(e->getEntities());
    for(unsigned int i = 0; i < ent.size(); i++){
      if(ent[i]->getSelection()){
        GVertex *gv = dynamic_cast<GVertex*>(ent[i]);
        if(!gv)
          Msg::Error("Problem in point selection processing");
        else
          project_point(ps, gv->x(), gv->y(), gv->z(), u, v, dist, f);
      }
    }
    std::vector<MElement*> &ele(e->getElements());
    std::set<MVertex*> verts;
    for(unsigned int i = 0; i < ele.size(); i++)
      if(ele[i]->getVisibility() == 2)
        for(int j = 0; j < ele[i]->getNumVertices(); j++)
          verts.insert(ele[i]->getVertex(j));
    for(std::set<MVertex*>::iterator it = verts.begin(); it != verts.end(); it++)
      project_point(ps, (*it)->x(), (*it)->y(), (*it)->z(), u, v, dist, f);
    e->uv()->set(u, v, dist, f);
  }

  drawContext::global()->draw();
}

static void browse_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;

  std::vector<projection*> &projections(e->getProjections());
  for(unsigned int i = 0; i < projections.size(); i++){
    projections[i]->face->setVisibility(false);
    projections[i]->group->hide();
  }

  projection *p = e->getCurrentProjection();
  if(p){
    p->face->setVisibility(true);
    p->group->show();
  }

  update_cb(0, data);
}

static void invert_normal_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  projection *p = e->getCurrentProjection();
  if(p){
    p->parameters[3]->value(-p->parameters[3]->value());
    p->parameters[4]->value(-p->parameters[4]->value());
    p->parameters[5]->value(-p->parameters[5]->value());
    update_cb(0, data);
  }
}

static void translate(void *data, int axis, bool plus)
{
  projectionEditor *e = (projectionEditor*)data;
  projection *p = e->getCurrentProjection();
  if(p){
    FM::ProjectionSurface *ps = (FM::ProjectionSurface*)p->face->getNativePtr();
    SVector3 origin(p->parameters[0]->value(),
                    p->parameters[1]->value(),
                    p->parameters[2]->value());
    SVector3 vec;
    if(axis == 0)
      ps->GetE0(vec[0], vec[1], vec[2]);
    else if(axis == 1)
      ps->GetE1(vec[0], vec[1], vec[2]);
    else
      ps->GetE2(vec[0], vec[1], vec[2]);
    if(plus)
      origin += vec * (CTX::instance()->lc / 100.);
    else
      origin -= vec * (CTX::instance()->lc / 100.);
    p->parameters[0]->value(origin[0]);
    p->parameters[1]->value(origin[1]);
    p->parameters[2]->value(origin[2]);
    update_cb(0, data);
  }
}

static void translate_p0_cb(Fl_Widget *w, void *data){ translate(data, 0, true); }
static void translate_p1_cb(Fl_Widget *w, void *data){ translate(data, 1, true); }
static void translate_p2_cb(Fl_Widget *w, void *data){ translate(data, 2, true); }
static void translate_m0_cb(Fl_Widget *w, void *data){ translate(data, 0, false); }
static void translate_m1_cb(Fl_Widget *w, void *data){ translate(data, 1, false); }
static void translate_m2_cb(Fl_Widget *w, void *data){ translate(data, 2, false); }

static void set_position_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  projection *p = e->getCurrentProjection();
  if(p){
    char ib = FlGui::instance()->selectEntity(ENT_ALL);
    if(ib == 'l'){
      if(FlGui::instance()->selectedVertices.size()){
        p->parameters[0]->value(FlGui::instance()->selectedVertices[0]->x());
        p->parameters[1]->value(FlGui::instance()->selectedVertices[0]->y());
        p->parameters[2]->value(FlGui::instance()->selectedVertices[0]->z());
      }
      else if(FlGui::instance()->selectedElements.size()){
        SPoint3 pc = FlGui::instance()->selectedElements[0]->barycenter();
        p->parameters[0]->value(pc.x());
        p->parameters[1]->value(pc.y());
        p->parameters[2]->value(pc.z());
        if(FlGui::instance()->selectedElements[0]->getNumFaces()){
          MFace f = FlGui::instance()->selectedElements[0]->getFace(0);
          SVector3 n = f.normal();
          p->parameters[3]->value(n[0]);
          p->parameters[4]->value(n[1]);
          p->parameters[5]->value(n[2]);
        }
      }
    }
    ((Fl_Toggle_Button*)w)->value(0);
    update_cb(0, data);
  }
}

static void select_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;

  int what = e->getSelectionMode();
  const char *str;

  switch(what){
  case ENT_ALL: CTX::instance()->pickElements = 1; str = "Elements"; break;
  case ENT_POINT: CTX::instance()->pickElements = 0; str = "Points"; break;
  case ENT_SURFACE: CTX::instance()->pickElements = 0; str = "Surfaces"; break;
  default: return;
  }

  std::vector<MElement*> &ele(e->getElements());
  std::vector<GEntity*> &ent(e->getEntities());

  while(1) {
    CTX::instance()->mesh.changed = ENT_ALL;
    drawContext::global()->draw();

    if(ele.size() || ent.size())
      Msg::StatusGl("Select %s\n[Press 'e' to end selection, 'u' to undo"
                    "last selection or 'q' to abort]", str);
    else
      Msg::StatusGl("Select %s\n"
                    "[Press 'e' to end selection or 'q' to abort]", str);

    char ib = FlGui::instance()->selectEntity(what);
    if(ib == 'l') {
      if(CTX::instance()->pickElements){
        for(unsigned int i = 0; i < FlGui::instance()->selectedElements.size(); i++){
          if(FlGui::instance()->selectedElements[i]->getVisibility() != 2){
            FlGui::instance()->selectedElements[i]->setVisibility(2);
            ele.push_back(FlGui::instance()->selectedElements[i]);
          }
        }
      }
      else{
        for(unsigned int i = 0; i < FlGui::instance()->selectedVertices.size(); i++){
          if(FlGui::instance()->selectedVertices[i]->getSelection() != 1){
            FlGui::instance()->selectedVertices[i]->setSelection(1);
            ent.push_back(FlGui::instance()->selectedVertices[i]);
          }
        }
        for(unsigned int i = 0; i < FlGui::instance()->selectedFaces.size(); i++){
          if(FlGui::instance()->selectedFaces[i]->getSelection() != 1){
            FlGui::instance()->selectedFaces[i]->setSelection(1);
            ent.push_back(FlGui::instance()->selectedFaces[i]);
          }
        }
      }
    }
    if(ib == 'r') {
      if(CTX::instance()->pickElements){
        for(unsigned int i = 0; i < FlGui::instance()->selectedElements.size(); i++)
          FlGui::instance()->selectedElements[i]->setVisibility(1);
      }
      else{
        for(unsigned int i = 0; i < FlGui::instance()->selectedVertices.size(); i++)
          FlGui::instance()->selectedVertices[i]->setSelection(0);
        for(unsigned int i = 0; i < FlGui::instance()->selectedFaces.size(); i++)
          FlGui::instance()->selectedFaces[i]->setSelection(0);
      }
    }
    if(ib == 'u') {
      if(CTX::instance()->pickElements){
        if(ele.size()){
          ele[ele.size() - 1]->setVisibility(1); ele.pop_back();
        }
      }
      else{
        if(ent.size()){
          ent[ent.size() - 1]->setSelection(0); ent.pop_back();
        }
      }
    }
    if(ib == 'e') {
      GModel::current()->setSelection(0);
      ele.clear();
      ent.clear();
    }
    if(ib == 'q') {
      GModel::current()->setSelection(0);
      ele.clear();
      ent.clear();
      break;
    }
    update_cb(0, data);
  }

  CTX::instance()->mesh.changed = ENT_ALL;
  CTX::instance()->pickElements = 0;
  drawContext::global()->draw();
  Msg::StatusGl("");
}

static void filter_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  projection *p = e->getCurrentProjection();
  if(p){
    SBoundingBox3d bbox = GModel::current()->bounds();
    double lc = norm(SVector3(bbox.max(), bbox.min()));
    double threshold = e->getThreshold() * lc;
    FM::ProjectionSurface *ps = (FM::ProjectionSurface*)p->face->getNativePtr();
    std::vector<GEntity*> &ent(e->getEntities());
    for(unsigned int i = 0; i < ent.size(); i++){
      GVertex *gv = dynamic_cast<GVertex*>(ent[i]);
      if(gv){
        double uu, vv, p[3];
        ps->OrthoProjectionOnSurface(gv->x(), gv->y(), gv->z(), uu, vv);
        ps->F(uu, vv, p[0], p[1], p[2]);
        double dx = gv->x() - p[0], dy = gv->y() - p[1], dz = gv->z() - p[2];
        if(uu >= 0. && uu <= 1. && vv >= 0. && vv < 1. &&
           sqrt(dx * dx + dy * dy + dz * dz) < threshold)
          gv->setSelection(true);
        else
          gv->setSelection(false);
      }
    }
    std::vector<MElement*> &ele(e->getElements());
    for(unsigned int i = 0; i < ele.size(); i++){
      SPoint3 pc = ele[i]->barycenter();
      double uu, vv, p[3];
      ps->OrthoProjectionOnSurface(pc.x(), pc.y(), pc.z(), uu, vv);
      ps->F(uu, vv, p[0], p[1], p[2]);
      double dx = pc.x() - p[0], dy = pc.y() - p[1], dz = pc.z() - p[2];
      if(uu >= 0. && uu <= 1. && vv >= 0. && vv < 1. &&
         sqrt(dx * dx + dy * dy + dz * dz) < threshold){
        ele[i]->setVisibility(2);
        // keep only the elements oriented in the same direction as
        // the projection surface
        if(e->getOrientation() && ele[i]->getNumFaces()){
          MFace f = ele[i]->getFace(0);
          SVector3 n = f.normal(), n2;
          ps->GetNormal(uu, vv, n2[0], n2[1], n2[2]);
          if(dot(n, n2) < 0.) ele[i]->setVisibility(1);
        }
      }
      else
        ele[i]->setVisibility(1);
    }
    if(ele.size()) CTX::instance()->mesh.changed = ENT_ALL;
  }
  update_cb(0, data);
}

static void proj_hide_cb(Fl_Widget *w, void *data)
{
  CTX::instance()->hideUnselected = !CTX::instance()->hideUnselected;
  CTX::instance()->mesh.changed = ENT_ALL;
  drawContext::global()->draw();
}

static void save_selection_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  if(fileChooser(FILE_CHOOSER_CREATE, "Save Selection", "*.{geo,msh}")){
    FILE *fp = Fopen(fileChooserGetName(1).c_str(), "w");
    if(!fp){
      Msg::Error("Unable to open file `%s'", fileChooserGetName(1).c_str());
      return;
    }
    std::vector<GEntity*> &ent(e->getEntities());
    for(unsigned int i = 0; i < ent.size(); i++){
      GVertex *gv = dynamic_cast<GVertex*>(ent[i]);
      if(gv && gv->getSelection())
        fprintf(fp, "Point(%d) = {%.16g,%.16g,%.16g,1};\n", gv->tag(),
                gv->x(), gv->y(), gv->z());
    }
    std::vector<MElement*> &ele(e->getElements());
    if(ele.size()){
      int nelm = 0;
      std::set<MVertex*> verts;
      for(unsigned int i = 0; i < ele.size(); i++){
        if(ele[i]->getVisibility() == 2){
          nelm++;
          for(int j = 0; j < ele[i]->getNumVertices(); j++)
            verts.insert(ele[i]->getVertex(j));
        }
      }
      fprintf(fp, "$NOD\n%d\n", (int)verts.size());
      for(std::set<MVertex*>::iterator it = verts.begin(); it != verts.end(); it++)
        (*it)->writeMSH(fp);
      fprintf(fp, "$ENDNOD\n$ELM\n%d\n", nelm);
      for(unsigned int i = 0; i < ele.size(); i++)
        if(ele[i]->getVisibility() == 2)
          ele[i]->writeMSH(fp, 1.0);
      fprintf(fp, "$ENDELM\n");
    }
    fclose(fp);
  }
}

static void load_projection_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  if(fileChooser(FILE_CHOOSER_SINGLE, "Load Projection", "*.pro")){
    FILE *fp = Fopen(fileChooserGetName(1).c_str(), "r");
    if(!fp){
      Msg::Error("Unable to open file `%s'", fileChooserGetName(1).c_str());
      return;
    }
    int num;
    if(!fscanf(fp, "%d", &num)){
      Msg::Error("Bad projection file format");
      fclose(fp);
      return;
    }
    for(int proj = 0; proj < num; proj++){
      char name[256], tag[256];
      if(!fscanf(fp, "%s", tag) || !fscanf(fp, "%s", name)){
        Msg::Error("Bad projection file format");
        fclose(fp);
        return;
      }
      fourierProjectionFace *face = createProjectionFaceFromName(name);
      if(face){
        e->load(face, tag);
        projection *p = e->getLastProjection();
        if(p){
          for(unsigned int i = 0; i < p->parameters.size(); i++){
            double val;
            if(!fscanf(fp, "%lf", &val)){
              Msg::Error("Missing paramater for projection `%s'", name);
              break;
            }
            p->parameters[i]->value(val);
          }
        }
      }
    }
    fclose(fp);
    update_cb(0, data);
  }
}

static void save_projection_cb(Fl_Widget *w, void *data)
{
  projectionEditor *e = (projectionEditor*)data;
  projection *p = e->getCurrentProjection();
  if(p){
    FM::ProjectionSurface *ps = (FM::ProjectionSurface*)p->face->getNativePtr();
    if(fileChooser(FILE_CHOOSER_CREATE, "Save Projection", "*.pro")){
      std::string name = fileChooserGetName(1);
      FILE *fp = Fopen(name.c_str(), "w");
      if(!fp){
        Msg::Error("Unable to open file `%s'", name.c_str());
        return;
      }
      fprintf(fp, "1\n%s\n%s\n", SplitFileName(name)[1].c_str(), ps->GetName().c_str());
      for(unsigned int i = 0; i < p->parameters.size(); i++)
        fprintf(fp, "%.16g\n", p->parameters[i]->value());
      fclose(fp);
    }
  }
}

static void compute_cb(Fl_Widget *w, void *data)
{
  GModel* m = GModel::current();

  printf("ngroups = %d\n",m->getFMInternals()->current()->GetNumGroups());

  projectionEditor *e = (projectionEditor*)data;

  projection *p = e->getCurrentProjection();
  if(p){
    // get the projection data
    std::vector<double> u, v, dist;
    std::vector<std::complex<double> > f;
    e->uv()->get(u, v, dist, f);
    if(f.empty()) return;

    int uModes = e->getMode(0);
    int vModes = e->getMode(1);

    if((int)f.size() < uModes * vModes){
      Msg::Error("Number of points < uModes * vModes");
      return;
    }

    int uM = e->getMode(2);
    int vM = e->getMode(3);
    int h0 = e->getHardEdge(0);
    int h1 = e->getHardEdge(1);
    int h2 = e->getHardEdge(2);
    int h3 = e->getHardEdge(3);

    FM::ProjectionSurface *ps = (FM::ProjectionSurface*)p->face->getNativePtr();
    if (e->getPatchType()) {
      // create the US-FFT/Windowing faces (with boundaries)
      FM::Patch* patch =
        new FM::WFPatch(0, ps->clone(), u, v, f, 3, uModes, vModes);
      m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
        AddPatch(patch);
      m->getFMInternals()->makeGFace(patch,m);
      //makeGFace(patch);
    }
    else {
      // create the Fourier faces (with boundaries)
      if(ps->IsUPeriodic()) {
        FM::Patch* patchL =
          new FM::FPatch(0, ps->clone(), u, v, f, 3, uModes, vModes,
                         uM, vM, h0, h1, h2, h3);
        patchL->SetMinU(-0.35);
        patchL->SetMaxU(0.35);
        m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
          AddPatch(patchL);
        m->getFMInternals()->makeGFace(patchL,m);
        //makeGFace(patchL);
        FM::Patch* patchR =
          new FM::FPatch(0, ps->clone(), u, v, f, 3, uModes, vModes,
                         uM, vM, h0, h1, h2, h3);
        patchR->SetMinU(0.15);
        patchR->SetMaxU(0.85);
        m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
          AddPatch(patchR);
        m->getFMInternals()->makeGFace(patchR,m);
        //makeGFace(patchR);
      }
      else if (ps->IsVPeriodic()) {
        FM::Patch* patchL =
          new FM::FPatch(0, ps->clone(), u, v, f, 3, uModes, vModes,
                         uM, vM, h0, h1, h2, h3);
        patchL->SetMinV(-0.35);
        patchL->SetMaxV(0.35);
        m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
          AddPatch(patchL);
        m->getFMInternals()->makeGFace(patchL,m);
        //makeGFace(patchL);
        FM::Patch* patchR =
          new FM::FPatch(0, ps->clone(), u, v, f, 3, uModes, vModes,
                         uM, vM, h0, h1, h2, h3);
        patchR->SetMinV(0.15);
        patchR->SetMaxV(0.85);
        m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
          AddPatch(patchR);
        m->getFMInternals()->makeGFace(patchR,m);
        //makeGFace(patchR);
      }
      else {
        FM::Patch* patch =
          new FM::FPatch(0, ps->clone(), u, v, f, 3, uModes, vModes,
                         uM, vM, h0, h1, h2, h3);
        m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->
          AddPatch(patch);
        m->getFMInternals()->makeGFace(patch,m);
        //makeGFace(patch);
      }
    }
  }
  printf("nPatches = %d\n",m->getFMInternals()->current()->GetGroup(0)->
         GetBlendGroup()->GetNumPatches());
  drawContext::global()->draw();
}

static void delete_fourier(GFace *gf)
{
  if(gf->getNativeType() != GEntity::FourierModel) return;

  GModel *m = GModel::current();

  // don't actually delete the data so we can add `undo' later
  std::list<GVertex*> vertices = gf->vertices();
  for(std::list<GVertex*>::iterator it = vertices.begin(); it != vertices.end(); it++)
    m->remove(*it);

  std::list<GEdge*> edges = gf->edges();
  for(std::list<GEdge*>::iterator it = edges.begin(); it != edges.end(); it++)
    m->remove(*it);

  m->remove(gf);
}

static void blend_cb(Fl_Widget *w, void *data)
{
  GModel *m = GModel::current();

  std::vector<GFace*> faces;
  for (GModel::fiter it = m->firstFace(); it != m->lastFace(); it++)
    if ((*it)->getNativeType() == GEntity::FourierModel)
      faces.push_back(*it);

  m->getFMInternals()->current()->GetGroup(0)->GetBlendGroup()->Blend();
  for (int i = 0; i < m->getFMInternals()->current()->GetGroup(0)->
         GetBlendGroup()->GetNumPatches(); i++)
    m->getFMInternals()->makeGFace(m->getFMInternals()->current()->
                                   GetGroup(0)->GetBlendGroup()->
                                   GetPatch(i),m);
  for (unsigned int i = 0; i < faces.size(); i++) {
    //delete_fourier(faces[i]);
    faces[i]->setVisibility(0, true);
  }
}

static void action_cb(Fl_Widget *w, void *data)
{
  std::string what((char*)data);
  std::vector<GFace*> faces;

  GModel *m = GModel::current();

  if(what == "delete_last" || what == "save_last"){
    int id = -1;
    for(GModel::fiter it = m->firstFace(); it != m->lastFace(); it++)
      if((*it)->getNativeType() == GEntity::FourierModel)
        id = std::max(id, (*it)->tag());
    if(id > 0) faces.push_back(m->getFaceByTag(id));
  }
  else if(what == "delete_all" || what == "save_all"){
    for(GModel::fiter it = m->firstFace(); it != m->lastFace(); it++)
      if((*it)->getNativeType() == GEntity::FourierModel)
        faces.push_back(*it);
  }
  else if(what == "delete_select" || what == "save_select"){
    Msg::StatusGl("Select Surface\n[Press 'e' to end selection 'q' to abort]");
    char ib = FlGui::instance()->selectEntity(ENT_SURFACE);
    if(ib == 'l') faces.insert(faces.end(),
                               FlGui::instance()->selectedFaces.begin(),
                               FlGui::instance()->selectedFaces.end());
    Msg::StatusGl("");
  }

  if(what[0] == 'd'){
    for(unsigned int i = 0; i < faces.size(); i++)
      delete_fourier(faces[i]);
  }
  else{
    if(fileChooser(FILE_CHOOSER_CREATE, "Save Fourier Model", "*.fm")){
      FILE *fp = Fopen(fileChooserGetName(1).c_str(), "w");
      if(!fp){
        Msg::Error("Unable to open file `%s'", fileChooserGetName(1).c_str());
        return;
      }
      fprintf(fp, "%d\n", (int)faces.size());
      for(unsigned int i = 0; i < faces.size(); i++){
        fourierFace* ff = (fourierFace*)faces[i];
        ((FM::TopoFace*)ff->getNativePtr())->GetPatch()->Export(fp);
      }
      fclose(fp);
    }
  }

  drawContext::global()->draw();
}

uvPlot::uvPlot(int x, int y, int w, int h, const char *l)
  : Fl_Window(x, y, w, h, l), _dmin(0.), _dmax(0.)
{
  ColorTable_InitParam(2, &_colorTable);
  ColorTable_Recompute(&_colorTable);
}

void uvPlot::set(std::vector<double> &u, std::vector<double> &v,
                 std::vector<double> &dist, std::vector<std::complex<double> > &f)
{
  _u = u;
  _v = v;
  _dist = dist;
  _f = f;
  if(dist.empty()){
    _dmin = _dmax = 0.;
  }
  else{
    _dmin = _dmax = dist[0];
    for(unsigned int i = 1; i < dist.size(); i++){
      _dmin = std::min(_dmin, dist[i]);
      _dmax = std::max(_dmax, dist[i]);
    }
  }
  redraw();
}

void uvPlot::color(double d)
{
  int index;
  if(_dmin == _dmax)
    index = _colorTable.size / 2;
  else
    index = (int)((d - _dmin) * (_colorTable.size - 1) / (_dmax - _dmin));
  unsigned int color = _colorTable.table[index];
  int r = CTX::instance()->unpackRed(color);
  int g = CTX::instance()->unpackGreen(color);
  int b = CTX::instance()->unpackBlue(color);
  fl_color(r, g, b);
}

void uvPlot::draw()
{
  // draw background
  fl_color(FL_WHITE);
  fl_rectf(0, 0, w(), h());

  // draw points in u,v space, colored by their distance to the
  // projection surface
  int pw = w();
  int ph = h() - (2 * FL_NORMAL_SIZE + 5);
  for(unsigned int i = 0; i < _u.size(); i++){
    int x = (int)(_u[i] * pw);
    int y = (int)(_v[i] * ph);
    color(_dist[i]);
    fl_rect(x, y, 3, 3);
  }

  // draw color bar
  for(int i = 0; i < w(); i++){
    int index = (int)(i * (_colorTable.size - 1) / w());
    unsigned int color = _colorTable.table[index];
    int r = CTX::instance()->unpackRed(color);
    int g = CTX::instance()->unpackGreen(color);
    int b = CTX::instance()->unpackBlue(color);
    fl_color(r, g, b);
    fl_line(i, ph, i, ph + 10);
  }

  // draw labels
  fl_color(FL_BLACK);
  fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
  static char min[256], max[256], pts[256];
  sprintf(min, "%g", _dmin);
  sprintf(max, "%g", _dmax);
  sprintf(pts, "[%d pts]", (int)_u.size());
  fl_draw(min, 5, h() - 5);
  fl_draw(pts, pw / 2 - (int)fl_width(pts) / 2, h() - 5);
  fl_draw(max, pw - (int)fl_width(max) - 5, h() - 5);
}

projection::projection(fourierProjectionFace *f, int x, int y, int w, int h,
                       int bb, int bh, projectionEditor *e)
  : face(f)
{
  group = new Fl_Scroll(x, y, w, h);
  SBoundingBox3d bounds = GModel::current()->bounds();
  FM::ProjectionSurface *ps = (FM::ProjectionSurface*)f->getNativePtr();

  Fl_Toggle_Button *b = new Fl_Toggle_Button(x, y, bb, bh, "Set position");
  b->callback(set_position_cb, e);

  { // origin is stored in parameters[0,1,2]
    SPoint3 pc = bounds.center();
    for(int i = 0; i < 3; i++){
      Fl_Value_Input *v = new Fl_Value_Input(x, y + (1 + i) * bh, bb, bh);
      parameters.push_back(v);
      v->maximum(bounds.max()[i] + 10. * CTX::instance()->lc);
      v->minimum(bounds.min()[i] - 10. * CTX::instance()->lc);
      v->step(CTX::instance()->lc / 100., 1);
      v->value(pc[i]);
      v->label((i == 0) ? "X" : (i == 1) ? "Y" : "Z");
    }
    ps->SetOrigin(pc[0], pc[1], pc[2]);
    Fl_Repeat_Button *bm[3], *bp[3];
    for(int i = 0; i < 3; i++){
      new Fl_Box(x + w - bb / 3 - bb / 6, y + (1 + i) * bh, bb / 8, bh,
                 (i == 0) ? "E0" : (i == 1) ? "E1" : "E2");
      bp[i] = new Fl_Repeat_Button(x + w - bb / 3, y + (1 + i) * bh,
                                   bb / 8, bh / 2, "+");
      bm[i] = new Fl_Repeat_Button(x + w - bb / 3, y + (1 + i) * bh + bh / 2,
                                   bb / 8, bh / 2, "-");
    }
    bp[0]->callback(translate_p0_cb, e);
    bp[1]->callback(translate_p1_cb, e);
    bp[2]->callback(translate_p2_cb, e);
    bm[0]->callback(translate_m0_cb, e);
    bm[1]->callback(translate_m1_cb, e);
    bm[2]->callback(translate_m2_cb, e);
  }
  { // normal is stored in parameters[3,4,5]
    Fl_Value_Input *v1 = new Fl_Value_Input(x, y + 4 * bh, bb / 3, bh);
    parameters.push_back(v1);
    v1->maximum(1.); v1->minimum(-1.); v1->step(0.01); v1->value(0.);
    Fl_Value_Input *v2 = new Fl_Value_Input(x + bb / 3, y + 4 * bh, bb / 3, bh);
    parameters.push_back(v2);
    v2->maximum(1.); v2->minimum(-1.); v2->step(0.01); v2->value(0.);
    Fl_Value_Input *v3 = new Fl_Value_Input(x + 2 * bb / 3, y + 4 * bh, bb - 2 * bb / 3, bh);
    parameters.push_back(v3);
    v3->maximum(1.); v3->minimum(-1.); v3->step(0.01); v3->value(1.);
    v3->label("Normal");
    Fl_Button *b = new Fl_Button(x + w - bb / 3, y + 4 * bh + bh / 4, bb / 8, bh / 2, "-");
    b->callback(invert_normal_cb, e);
    b->tooltip("Invert normal");
  }
  { // rotation is stored in parameters[6]
    Fl_Value_Input *v = new Fl_Value_Input(x, y + 5 * bh, bb, bh, "Rotation");
    v->maximum(-180.);
    v->minimum(180.);
    v->step(0.1);
    v->value(0.);
    parameters.push_back(v);
  }
  { // scale is stored in parameters[7,8,9]
    for(int i = 0; i < 3; i++){
      Fl_Value_Input *v = new Fl_Value_Input(x, y + (6 + i) * bh, bb, bh);
      parameters.push_back(v);
      v->maximum(CTX::instance()->lc * 10.);
      v->minimum(CTX::instance()->lc / 100.);
      v->step(CTX::instance()->lc / 100., 1);
      v->value(CTX::instance()->lc / 10.);
      v->label((i == 0) ? "Scale0" : (i == 1) ? "Scale1" : "Scale2");
    }
  }

  // other parameters are stored in parameters[10,...]
  for(int i = 0; i < ps->GetNumParameters(); i++){
    Fl_Value_Input *v = new Fl_Value_Input(x, y + (9 + i) * bh, bb, bh);
    v->maximum(10. * CTX::instance()->lc);
    v->minimum(-10. * CTX::instance()->lc);
    v->step(CTX::instance()->lc / 100., 1);
    v->label(strdup(ps->GetLabel(i).c_str()));
    v->value(ps->GetParameter(i));
    parameters.push_back(v);
  }

  for(unsigned int i = 0; i < parameters.size(); i++){
    parameters[i]->align(FL_ALIGN_RIGHT);
    parameters[i]->callback(update_cb, e);
  }

  group->end();
  group->hide();
}

projectionEditor::projectionEditor()
{
  GModel *m = GModel::current();

  // construct FM_Internals
  m->readFourier();
  printf("readerSize = %d\n",m->getFMInternals()->getSize());
  printf("currentSize = %d\n",m->getFMInternals()->current()->GetNumGroups());

  // construct GUI in terms of standard sizes
  const int width = (int)(3.75 * BB), height = 24 * BH;

  // create all widgets (we construct this once, we never deallocate!)
  _window = new paletteWindow
    (width, height, CTX::instance()->nonModalWindows ? true : false, "Reparameterize");

  new Fl_Box(WB, WB + BH / 2, BB / 2, BH, "Select:");

  Fl_Group *o = new Fl_Group(WB, WB, 2 * BB, 3 * BH);
  _select[0] = new Fl_Round_Button(2 * WB + BB / 2, WB, BB, BH, "Points");
  _select[1] = new Fl_Round_Button(2 * WB + BB / 2, WB + BH, BB, BH, "Elements");
  if(m->getNumMeshElements())
    _select[1]->value(1);
  else
    _select[0]->value(1);
  for(int i = 0; i < 2; i++){
    _select[i]->callback(select_cb, this);
    _select[i]->type(FL_RADIO_BUTTON);
  }
  o->end();

  {
    Fl_Toggle_Button *b1 = new Fl_Toggle_Button
      (width - WB - 3 * BB / 2, WB, 3 * BB / 2, BH, "Hide unselected");
    b1->callback(proj_hide_cb);
    Fl_Button *b2 = new Fl_Button
      (width - WB - 3 * BB / 2, WB + BH, 3 * BB / 2, BH, "Save selection");
    b2->callback(save_selection_cb, this);
  }

  const int brw = (int)(1.3 * BB);

  _browser = new Fl_Hold_Browser(WB, 2 * WB + 2 * BH, brw, 6 * BH);
  _browser->callback(browse_cb, this);

  _paramWin[0] = 2 * WB + brw;
  _paramWin[1] = 2 * WB + 2 * BH;
  _paramWin[2] = width - 3 * WB - brw;
  _paramWin[3] = 7 * BH;
  _paramWin[4] = (int)(1.25 * BB);
  _paramWin[5] = BH;

  {
    Fl_Button *b1 = new Fl_Button(WB, 2 * WB + 8 * BH, brw / 2, BH, "Load");
    b1->callback(load_projection_cb, this);
    Fl_Button *b2 = new Fl_Button(WB + brw / 2, 2 * WB + 8 * BH, brw / 2, BH, "Save");
    b2->callback(save_projection_cb, this);
  }

  int hard = 8;
  int uvw = width - 2 * WB - 2 * hard - 3 * WB;
  int uvh = height - 7 * WB - 14 * BH - 2 * hard;

  _hardEdges[0] = new Fl_Toggle_Button(WB, 3 * WB + 9 * BH + hard,
                                       hard, uvh);
  _hardEdges[1] = new Fl_Toggle_Button(width - 4 * WB - hard, 3 * WB + 9 * BH + hard,
                                       hard, uvh);
  _hardEdges[2] = new Fl_Toggle_Button(WB + hard, 3 * WB + 9 * BH,
                                       uvw, hard);
  _hardEdges[3] = new Fl_Toggle_Button(WB + hard, height - 4 * WB - 5 * BH - hard,
                                       uvw, hard);
  for(int i = 0; i < 4; i++)
    _hardEdges[i]->tooltip("Push to mark edge as `hard'");

  _uvPlot = new uvPlot(WB + hard, 3 * WB + 9 * BH + hard, uvw, uvh);
  _uvPlot->end();

  _slider = new Fl_Slider(width - 3 * WB, 3 * WB + 9 * BH + hard, 2 * WB, uvh);
  _slider->minimum(1.);
  _slider->maximum(0.);
  _slider->value(1.);
  _slider->callback(filter_cb, this);
  _slider->tooltip("Filter selection by distance to projection surface");

  _orientation = new Fl_Toggle_Button(width - 3 * WB, height - 4 * WB - 5 * BH - hard,
                                      2 * WB, hard);
  _orientation->callback(filter_cb, this);
  _orientation->tooltip("Filter elements using orientation");

  new Fl_Box(WB, height - 3 * WB - 5 * BH, BB, BH, "Patch Type:");
  Fl_Group *oo = new Fl_Group(WB, height - 3 * WB - 5 * BH, 3 * BB, BH);
  _pselect[0] = new Fl_Round_Button
    (2 * WB + BB, height - 3 * WB - 5 * BH, BB, BH, "Continuation");
  _pselect[1] = new Fl_Round_Button
    (3 * WB + 2 * BB, height - 3 * WB - 5 * BH, BB, BH, "Windowing");

  for(int i = 0; i < 2; i++)
    _pselect[i]->type(FL_RADIO_BUTTON);

  _pselect[0]->value(1);

  oo->end();

  _modes[0] = new Fl_Value_Input(WB, height - 3 * WB - 4 * BH, BB  / 2, BH);
  _modes[0]->tooltip("Number of Fourier modes along u");
  _modes[1] = new Fl_Value_Input(WB + BB / 2, height - 3 * WB - 4 * BH, BB  / 2, BH,
                                 "Fourier modes");
  _modes[1]->tooltip("Number of Fourier modes along v");
  _modes[2] = new Fl_Value_Input(WB, height - 3 * WB - 3 * BH, BB  / 2, BH);
  _modes[2]->tooltip("Number of Chebyshev modes along u");
  _modes[3] = new Fl_Value_Input(WB + BB / 2, height - 3 * WB - 3 * BH, BB  / 2, BH,
                                 "Chebyshev modes");
  _modes[3]->tooltip("Number of Chebyshev modes along v");
  for(int i = 0; i < 4; i++){
    _modes[i]->value(8);
    _modes[i]->maximum(128);
    _modes[i]->minimum(1);
    _modes[i]->step(1);
    _modes[i]->align(FL_ALIGN_RIGHT);
  }

  {
    Fl_Button *b = new Fl_Button(width - WB - BB, height - 3 * WB - 4 * BH,
                                  BB, 2 * BH, "Generate\nPatch");
    b->callback(compute_cb, this);
  }

  {
    int bb = (int)(0.37 * BB);
    new Fl_Box(WB, height - 2 * WB - 2 * BH, BB / 2, BH, "Delete:");
    Fl_Button *b1 = new Fl_Button(WB + BB / 2, height - 2 * WB - 2 * BH,
                                  bb, BH, "last");
    b1->callback(action_cb, (void*)"delete_last");
    Fl_Button *b2 = new Fl_Button(WB + BB / 2 + bb, height - 2 * WB - 2 * BH,
                                  bb, BH, "all");
    b2->callback(action_cb, (void*)"delete_all");
    Fl_Button *b3 = new Fl_Button(WB + BB / 2 + 2 * bb, height - 2 * WB - 2 * BH,
                                  bb, BH, "sel.");
    b3->callback(action_cb, (void*)"delete_select");
  }

  {
    int bb = (int)(0.37 * BB);
    int s = width - WB - BB / 2 - 3 * bb;
    new Fl_Box(s, height - 2 * WB - 2 * BH, BB / 2, BH, "Save:");
    Fl_Button *b1 = new Fl_Button(s + BB / 2, height - 2 * WB - 2 * BH,
                                  bb, BH, "last");
    b1->callback(action_cb, (void*)"save_last");
    Fl_Button *b2 = new Fl_Button(s + BB / 2 + bb, height - 2 * WB - 2 * BH,
                                  bb, BH, "all");
    b2->callback(action_cb, (void*)"save_all");
    Fl_Button *b3 = new Fl_Button(s + BB / 2 + 2 * bb, height - 2 * WB - 2 * BH,
                                  bb, BH, "sel.");
    b3->callback(action_cb, (void*)"save_select");
  }

  {
    Fl_Button *b1 = new Fl_Button(WB, height - WB - BH, BB, BH,
                                  "Blend");
    b1->callback(blend_cb, this);

    //Fl_Button *b2 = new Fl_Button(2 * WB + BB, height - WB - BH, BB,
    //                            BH, "Intersect");
  }

  _window->end();
  _window->hotspot(_window);
  _window->resizable(_uvPlot);
  _window->size_range(width, (int)(0.85 * height));
}

void projectionEditor::load(fourierProjectionFace *face, std::string tag)
{
  FM::ProjectionSurface *ps = (FM::ProjectionSurface*)face->getNativePtr();
  _browser->add(tag.size() ? tag.c_str() : ps->GetName().c_str());
  projection *p =  new projection(face, _paramWin[0], _paramWin[1], _paramWin[2],
                                  _paramWin[3], _paramWin[4], _paramWin[5], this);
  _projections.push_back(p);
  _window->add(p->group);
}

void projectionEditor::show()
{
  _window->show();
  select_cb(0, this);
}

int projectionEditor::getSelectionMode()
{
  if(_select[0]->value())
    return ENT_POINT;
  else
    return ENT_ALL;
}

int projectionEditor::getPatchType()
{
  if (_pselect[0]->value())
    return 0;
  else
    return 1;
}

projection *projectionEditor::getCurrentProjection()
{
  for(int i = 1; i <= _browser->size(); i++)
    if(_browser->selected(i)) return _projections[i - 1];
  return 0;
}

projection *projectionEditor::getLastProjection()
{
  return _projections[_projections.size() - 1];
}

void mesh_parameterize_cb(Fl_Widget* w, void* data)
{
  // display geometry surfaces
  opt_geometry_surfaces(0, GMSH_SET | GMSH_GUI, 1);

  // create the (static) editor
  static projectionEditor *editor = 0;
  if(!editor){
    editor = new projectionEditor();
    editor->load(createProjectionFaceFromName("plane"));
    editor->load(createProjectionFaceFromName("trapezoid"));
    editor->load(createProjectionFaceFromName("sphere"));
    editor->load(createProjectionFaceFromName("paraboloid"));
    editor->load(createProjectionFaceFromName("cylinder"));
    editor->load(createProjectionFaceFromName("revolvedParabola"));
    editor->load(createProjectionFaceFromName("translatedParabola"));
  }
  editor->show();
}

#else

void mesh_parameterize_cb(Fl_Widget* w, void* data)
{
  Msg::Error("You must compile FourierModel to reparameterize meshes");
}

#endif
