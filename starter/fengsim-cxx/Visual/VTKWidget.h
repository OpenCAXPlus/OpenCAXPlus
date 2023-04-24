#ifndef VISUALWIDGET_H
#define VISUALWIDGET_H

#include "IVtkOCC_Shape.hxx"
#include "IVtkTools_ShapeDataSource.hxx"
#include "IVtkTools_ShapePicker.hxx"
#include "QVTKOpenGLWidget.h"
#include "vtkActor.h"
#include "vtkAxesActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include <QMouseEvent>

#include "CAD/CADDockWidget.h"
#include "CAD/PhysicsDockWidget.h"
#include "CAD/Primitive.h"

#include "vtkBoundingBox.h"
#include "vtkProperty.h"
#include "vtkSTLReader.h"
#include "vtkSimplePointsReader.h"
#include "vtkSimplePointsWriter.h"
#include "vtkTextActor.h"
#include "vtkTextActor3D.h"
#include "vtkTextProperty.h"
#include "vtkTransformFilter.h"
#include "vtkVoxelGrid.h"

class VTKWidget : public QVTKOpenGLWidget
{
public:
    VTKWidget(QWidget* parent = 0);
    static void ActorCreateIdSelected(vtkSmartPointer<vtkActor> actor, int ObjectId);
    static int  ActorGetId(vtkSmartPointer<vtkActor> actor);
    static bool ActorIsSelected(vtkSmartPointer<vtkActor> actor);
    static void ActorSetSelected(vtkSmartPointer<vtkActor> actor, bool selected);
    // add
    void Plot(TopoDS_Shape S, bool t = true);
    void RePlot() { GetRenderWindow()->Render(); }
    // select
    void       ExportCurrentCamera();
    void       SetSelectable(bool t) { Selectable = t; }
    void       SetSelectBnd(bool t) { SelectBnd = t; }
    void       SetSelectDomain(bool t) { SelectDomain = t; }
    int        SelectedId();
    vtkActor*  GetSelectedActor() { return SelectedVtkActor; }
    Primitive* GetSelectedPrim();
    Boundary*  GetSelectedBnd();
    int        GetSelectedBndId();
    // remove
    void Remove(vtkActor* A);
    void Remove();
    void Hide();
    void Reset();
    void Clear();
    void ImportVTKFile(std::string name);

    void ImportVTKFileCloudColorFinal(double t = 0.05);
    // additive manufacturing
    void ImportVTKFileAMStlModel(std::string name);
    void ImportVTKFileAMSlices(std::string name);
    void ImportVTKFileAMMesh(std::string name);
    void ImportVTKFileAMPathPlanning(std::string name);
    void ClearAMStlModel() { Clear(actor_am_stl_model); }
    void ShowAMStlModel(bool t)
    {
        if (t) {
            actor_am_stl_model->VisibilityOn();
        } else {
            actor_am_stl_model->VisibilityOff();
        }
        GetRenderWindow()->Render();
    }
    void ClearAMSlices() { Clear(actor_am_slices); }
    void ShowAMSlices(bool t)
    {
        if (t) {
            actor_am_slices->VisibilityOn();
        } else {
            actor_am_slices->VisibilityOff();
        }
        GetRenderWindow()->Render();
    }
    void ClearAMPathPlanning() { Clear(actor_am_path_planning); }
    void ShowAMPathPlanning(bool t)
    {
        if (t) {
            actor_am_path_planning->VisibilityOn();
        } else {
            actor_am_path_planning->VisibilityOff();
        }
        GetRenderWindow()->Render();
    }
    void ClearAMMesh() { Clear(actor_am_mesh); }
    void ShowAMMesh(bool t)
    {
        if (t) {
            actor_am_mesh->VisibilityOn();
        } else {
            actor_am_mesh->VisibilityOff();
        }
        GetRenderWindow()->Render();
    }

    void ShowCloudSource(bool t)
    {
        if (t)
            meas_cloud_source_actor->VisibilityOn();
        else
            meas_cloud_source_actor->VisibilityOff();
        GetRenderWindow()->Render();
    }
    void ClearCloudTarget() { Clear(meas_cloud_target_actor); }
    void ShowCloudTarget(bool t)
    {
        if (t)
            meas_cloud_target_actor->VisibilityOn();
        else
            meas_cloud_target_actor->VisibilityOff();
        GetRenderWindow()->Render();
    }
    void ShowCloudScene(bool t)
    {
        if (t)
            meas_scene->VisibilityOn();
        else
            meas_scene->VisibilityOff();
        GetRenderWindow()->Render();
    }

    void ClearCloudFinalColor() { Clear(meas_icp_final_color); }
    void Clear(vtkSmartPointer<vtkActor> _actor)
    {
        vtkActorCollection* acts = renderer->GetActors();
        int                 num  = acts->GetNumberOfItems();
        acts->InitTraversal();
        for (int i = 0; i < num; ++i) {
            vtkActor* actor = acts->GetNextActor();
            if (actor = _actor) renderer->RemoveActor(actor);
        }
        // aPicker = vtkSmartPointer<IVtkTools_ShapePicker>::New();
        // aPicker->SetRenderer(renderer);
        GetRenderWindow()->Render();
    }
    void ImportSTLFile(std::string name);
    void SetPrims(Primitives* p) { prims = p; }
    void SetBnds(Boundaries* p) { bnds = p; }
    void PlotBnds();
    void PlotDomains();
    void UpdateBndValue();
    // view
    void SetCADDockWidget(CADDockWidget* dw) { cad_dw = dw; }
    void SetPhyDockWidget(PhysicsDockWidget* dw) { phy_dw = dw; }
    void Fit();
    void Front();
    void Back();
    void Top();
    void Bottom();
    void Left();
    void Right();
    void Axo();
    void ViewRotationH();
    void ViewRotationV();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    void         Pick(double x, double y);
    //  virtual void resizeEvent( QResizeEvent* );
    //  virtual void mousePressEvent( QMouseEvent* );
    //  virtual void mouseReleaseEvent(QMouseEvent* );
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void MouseMove(Qt::MouseButtons nFlags, const QPoint point);
    // virtual void wheelEvent( QWheelEvent* );
private:
    vtkSmartPointer<vtkRenderer>           renderer;
    vtkSmartPointer<IVtkTools_ShapePicker> aPicker;
    Primitives*                            prims;
    Boundaries*                            bnds;
    int                                    ObjectId;
    vtkActor*                              SelectedVtkActor;
    bool                                   Selectable;
    bool                                   SelectBnd;
    bool                                   SelectDomain;
    QTableWidget*                          Properties;
    CADDockWidget*                         cad_dw;
    PhysicsDockWidget*                     phy_dw;

    // *******************************************************
    // general
public:
    std::vector<int> selected_bnd_id;
    void             ClearSelectedBnd();

    // *******************************************************
    // measure
private:
    vtkSmartPointer<vtkActor> meas_cad_actor;
    vtkSmartPointer<vtkActor> meas_cloud_target_actor;
    vtkSmartPointer<vtkActor> meas_cloud_source_actor;
    vtkSmartPointer<vtkActor> meas_cloud_icp_actor;
    vtkSmartPointer<vtkActor> meas_icp_final;
    vtkSmartPointer<vtkActor> meas_icp_final_color;
    vtkSmartPointer<vtkActor> meas_gdt_actor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkActor> meas_model;
    vtkSmartPointer<vtkActor> meas_scene;
    std::vector<vtkSmartPointer<vtkActor>> meas_bnd_actors;
    vtkSmartPointer<vtkSTLReader> meas_reader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkVoxelGrid> meas_us     = vtkSmartPointer<vtkVoxelGrid>::New();
    double                        meas_target[3];
    double                        meas_source[3];
    int                           meas_scene_tri_num;
    std::vector<int>              meas_deleted_bnd;

public:
    void MeasureImportCloudTarget(std::string name, int r = 0, int g = 0, int b = 255);
    void MeasureImportCloudICP(std::string name, int r = 0, int g = 255, int b = 0);
    void MeasureImportCloudSource2(std::string name, int r = 0, int g = 0, int b = 255);
    bool MeasureImportCloudSource(std::string name);
    bool MeasureExportCloudSource(std::string name,
                                  double      x       = 0,
                                  double      y       = 0,
                                  double      z       = 0,
                                  double      angle_x = 0,
                                  double      angle_y = 0,
                                  double      angle_z = 0);
    void MeasureCloudSourceTransform(double  x        = 0,
                                     double  y        = 0,
                                     double  z        = 0,
                                     double  angle_x  = 0,
                                     double  angle_y  = 0,
                                     double  angle_z  = 0,
                                     QString filename = "./data/meas");
    void MeasureClearCloudTarget();
    void MeasureClearCloudSource();
    void MeasureClearAll();
    void MeasureImportICPFinal(std::string name);
    void MeasureImportColorICPFinal(std::string name);
    void MeasureClearICPFinal();
    void MeasureSetSelectedBndsUnvisible();
    void MeasureVTKICP();
    void MeasureExportModel();
    void MeasureImportSource(std::string name, QString path);
    void MeasureImportSource2(QString path);
    void MeasurePlotCAD(TopoDS_Shape S, bool t = true);
    void MeasurePlotCAD2(TopoDS_Shape S, bool t = true);
    void MeasurePlotBnds();
    void MeasurePlotDomains();
    int  MeasureGetSceneTriNum() { return meas_scene_tri_num; }
    void MeasureCADHide();
    void MeasureCADOn();
    void MeasureBndsHide();
    void MeasureBndsOn();
    void MeasureCloudTargetHide(bool hide);
    void MeasureCloudSourceHide(bool hide);
    void MeasureCloudICPHide(bool hide);
    void MeasureCloudGDTHide(bool hide);

    void MeasureDeleteBnd();
    bool MeasureIsDelete(int id)
    {
        for (int i = 0; i < meas_deleted_bnd.size(); i++) {
            if (meas_deleted_bnd[i] == id) return true;
        }
        return false;
    }
    void   MeasureOpacity(double p);
    double measure_op = 1;

    void                                MeasureSTLTransform(QString path);
    void                                MeasureSTLTransform2(QString path);
    vtkSmartPointer<vtkTransformFilter> meas_transformFilter =
        vtkSmartPointer<vtkTransformFilter>::New();
    vtkSmartPointer<vtkTransformFilter> meas_un_transformFilter =
        vtkSmartPointer<vtkTransformFilter>::New();
    double meas_source_density;

    int  MeasureTransformCellsNumber();
    int  MeasureTransformDensity() { return meas_source_density; }
    void MeasureTransformCell(int i, int j, double& x, double& y, double& z);
    void MeasuresPlotICPtrans(double r, double g, double b);

    //        double* MeasureCloudTargetCenter () {
    //        meas_cloud_target_actor->GetCenter();} double* MeasureCloudSourceCenter ()
    //        { meas_cloud_source_actor->GetCenter();} void MeasureCenterFit (double
    //        x=0, double y=0, double z=0) {
    //                double target_center[3];
    //                double source_center[3];
    //                for (int i=0; i<3; i++) {
    //                        target_center[i] = MeasureCloudTargetCenter()[i];
    //                        source_center[i] = MeasureCloudSourceCenter()[i];
    //                }
    //                std::cout << "before center fit" << std::endl;
    //                std::cout << target_center[0] << " "  << target_center[1] << " "
    //                << target_center[2] << std::endl; std::cout << source_center[0] <<
    //                " "  << source_center[1] << " "  << source_center[2] << std::endl;

    //                MeasureCloudSourceTransform(target_center[0] - source_center[0],
    //                                target_center[1] - source_center[1],
    //                                target_center[2] - source_center[2]
    //                                );

    //                //meas_cloud_source_actor->SetPosition(target_center[0],
    //                target_center[1], target_center[2]);

    //                std::cout << "after center fit" << std::endl;
    //                std::cout << target_center[0] << " "  << target_center[1] << " "
    //                << target_center[2] << std::endl; std::cout << source_center[0] <<
    //                " "  << source_center[1] << " "  << source_center[2] << std::endl;

    //        }
    //        double* MeasureCloudSourceOrigin () {
    //                return meas_cloud_source_actor->GetOrigin();
    //        }

    // *******************************************************
    // additive manufacturing
private:
    vtkSmartPointer<vtkActor> am_cad_actor;
    vtkSmartPointer<vtkActor> am_stl_actor;
    vtkSmartPointer<vtkActor> am_slices_actor;
    vtkSmartPointer<vtkActor> am_pathplanning_actor;
    vtkSmartPointer<vtkActor> am_mesh_actor;
    vtkSmartPointer<vtkActor> am_simulation_actor;
    vtkSmartPointer<vtkActor> am_source_actor;

    vtkSmartPointer<vtkActor> actor_am_stl_model;
    vtkSmartPointer<vtkActor> actor_am_slices;
    vtkSmartPointer<vtkActor> actor_am_mesh;
    vtkSmartPointer<vtkActor> actor_am_path_planning;

public:
    void AMImportCAD(TopoDS_Shape S, bool t = true);
    void AMSetCADVisible(bool t);
    void AMImportSTL();
    void AMSetSTLVisible(bool t);
    void AMImportSlices();
    void AMSetSlicesVisible(bool t);
    void AMImportPathPlanning();
    void AMSetPathPlanningVisible(bool t);
    void AMImportMesh();
    void AMSetMeshVisible(bool t);
    void AMImportSimulation(int num);
    void AMImportSource(int num);

    // *******************************************************
    // fem
private:
    vtkSmartPointer<vtkActor> fem_simulation_actor;

public:
    void FEMImportResults(QString filename);
    void FEMImportResults2(QString filename);

    // *******************************************************
    // machining
private:
    vtkSmartPointer<vtkActor> machining_part_actor;
    vtkSmartPointer<vtkActor> machining_tool_actor;
    Boundaries*               machining_part_bnds;

public:
    void MachiningToolPlot(TopoDS_Shape S, bool t = true);
    void MachiningPartPlot(TopoDS_Shape S, bool t = true);
    void MachiningPlotPartBnds();
    void machining_set_part_bnds(Boundaries* bnds) { machining_part_bnds = bnds; }

    void MachiningImportPart(int num);
    void MachiningImportTool(int num);

    // *******************************************************
    // transport
public:
    void TransportPlot(TopoDS_Shape S, bool t = true, int color = 0);
    void TransportImportVTKFile(std::string name, int color = 0);

    // *******************************************************
    // ocporo
public:
    int OCPoroImportVTKFile(std::string name, int n = 0);

    // *******************************************************
    // text output
    // Setup the text and add it to the renderer
public:
    std::vector<QString>          vtktext;
    vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
    void TextOutput(QString str = QString("Hi, Welcome to FENGSim! :-)"))
    {
        vtktext.push_back(str);
        int     n = vtktext.size();
        QString textout;
        if (n < 5) {
            for (int i = 0; i < n - 1; i++) {
                textout += vtktext[i];
                textout += "\n";
            }
            textout += vtktext[n - 1];
        } else {
            for (int i = 0; i < 4; i++) {
                textout += vtktext[n + i - 5];
                textout += "\n";
            }
            textout += vtktext[n - 1];
        }
        textActor->SetInput(textout.toStdString().c_str());
        GetRenderWindow()->Render();
    }
    void SetTextPosition();
};

#endif // VISUALWIDGET_H
