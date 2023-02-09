#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T12:40:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FENGSim
TEMPLATE = app

include(CAD/cad.pri)
include(Measure/measure.pri)
include(OCPoro/ocporo.pri)
include(Machining/machining.pri)
include(Mesh/mesh.pri)
include(AdditiveManufacturing/am.pri)
include(Transport/transport.pri)
include(Visual/visual.pri)
include(FEM/fem.pri)
include(StatisticalProcessControl/spc.pri)

SOURCES +=\
    Main.cpp \
    MainWindow.cpp \
    qcustomplot.cpp \
    AboutDialog.cpp \

HEADERS  += \
    MainWindow.h \
    qcustomplot.h \
    AboutDialog.h \
    vtkocc.h \

FORMS    += \
    MainWindow.ui \
    AboutDialog.ui \

RESOURCES += \
    MainWindow.qrc \
    MeshWindow.qrc \
    FEMWindow.qrc \
    CADWindow.qrc \
    DataBaseWindow.qrc \
    MeasureWidnow.qrc \
    OCPoro.qrc \
    System.qrc \
    SPCWindow.qrc \
    AMWindow.qrc \
    Machining/machining.qrc \
    figure/transport_wind/transportwindow.qrc



INCLUDEPATH += /usr/local/include/oce /usr/local/include/gmsh /usr/local/include/vtk-8.1 /usr/local/include/pcl-1.8 /usr/local/include/Geant4 /home/jiping/FENGSim/FENGSim/GDT/
INCLUDEPATH += \
$$PWD/../../toolkit/OCE/external/install/OCE-0.18.3/include/oce \
$$PWD/../../toolkit/gmsh/external/install/gmsh-3.0.6/include/gmsh \
$$PWD/../../toolkit/VTK/external/install/VTK-8.1.0/include/vtk-8.1 

LIBS += -L/usr/local/lib \
-L$$PWD/../../toolkit/OCE/external/install/OCE-0.18.3/lib \
-L$$PWD/../../toolkit/gmsh/external/install/gmsh-3.0.6/lib \
-L$$PWD/../../toolkit/VTK/external/install/VTK-8.1.0/lib \
-L$$PWD/../../toolkit/lapack/external/install/lapack-3.11.0/lib \
-lTKBinL     -lTKBO     -lTKFeat      -lTKGeomBase  -lTKMath    -lTKOpenGl  -lTKService      -lTKSTEP209   -lTKTObj     -lTKXCAFSchema  -lTKXmlL \
-lTKBin      -lTKBRep   -lTKFillet    -lTKHLR       -lTKMesh    -lTKPCAF    -lTKShapeSchema  -lTKSTEPAttr  -lTKTopAlgo  -lTKXCAF        -lTKXml \
-lTKBinTObj  -lTKCAF    -lTKG2d       -lTKIGES      -lTKMeshVS  -lTKPLCAF   -lTKShHealing    -lTKSTEPBase  -lTKV3d      -lTKXDEIGES     -lTKXmlTObj \
-lTKBinXCAF  -lTKCDF    -lTKG3d       -lTKIVtk      -lTKNIS     -lTKPrim    -lTKStdLSchema   -lTKSTEP      -lTKVoxel    -lTKXDESTEP     -lTKXmlXCAF \
-lTKBool     -lTKernel  -lTKGeomAlgo  -lTKLCAF      -lTKOffset  -lTKPShape  -lTKStdSchema    -lTKSTL       -lTKVRML     -lTKXMesh       -lTKXSBase \
-lGmsh \
-lvtkalglib-8.1                       -lvtkFiltersHyperTree-8.1        -lvtkImagingFourier-8.1        -lvtkIOLSDyna-8.1        -lvtkRenderingAnnotation-8.1 \
-lvtkChartsCore-8.1                   -lvtkFiltersImaging-8.1          -lvtkImagingGeneral-8.1        -lvtkIOMINC-8.1          -lvtkRenderingContext2D-8.1 \
-lvtkCommonColor-8.1                  -lvtkFiltersModeling-8.1         -lvtkImagingHybrid-8.1         -lvtkIOMovie-8.1         -lvtkRenderingContextOpenGL2-8.1 \
-lvtkCommonComputationalGeometry-8.1  -lvtkFiltersParallel-8.1         -lvtkImagingMath-8.1           -lvtkIONetCDF-8.1        -lvtkRenderingCore-8.1 \
-lvtkCommonCore-8.1                   -lvtkFiltersParallelImaging-8.1  -lvtkImagingMorphological-8.1  -lvtkIOParallel-8.1      -lvtkRenderingFreeType-8.1 \
-lvtkCommonDataModel-8.1              -lvtkFiltersPoints-8.1           -lvtkImagingSources-8.1        -lvtkIOParallelXML-8.1   -lvtkRenderingGL2PSOpenGL2-8.1 \
-lvtkCommonExecutionModel-8.1         -lvtkFiltersProgrammable-8.1     -lvtkImagingStatistics-8.1     -lvtkIOPLY-8.1           -lvtkRenderingImage-8.1 \
-lvtkCommonMath-8.1                   -lvtkFiltersSelection-8.1        -lvtkImagingStencil-8.1        -lvtkIOSQL-8.1           -lvtkRenderingLabel-8.1 \
-lvtkCommonMisc-8.1                   -lvtkFiltersSMP-8.1              -lvtkInfovisCore-8.1           -lvtkIOTecplotTable-8.1  -lvtkRenderingLOD-8.1 \
-lvtkCommonSystem-8.1                 -lvtkFiltersSources-8.1          -lvtkInfovisLayout-8.1         -lvtkIOVideo-8.1         -lvtkRenderingOpenGL2-8.1 \
-lvtkCommonTransforms-8.1             -lvtkFiltersStatistics-8.1       -lvtkInteractionImage-8.1      -lvtkIOXML-8.1           -lvtkRenderingQt-8.1 \
-lvtkDICOMParser-8.1                  -lvtkFiltersTexture-8.1          -lvtkInteractionStyle-8.1      -lvtkIOXMLParser-8.1     -lvtkRenderingVolume-8.1 \
-lvtkDomainsChemistry-8.1             -lvtkFiltersTopology-8.1         -lvtkInteractionWidgets-8.1    -lvtkjpeg-8.1            -lvtkRenderingVolumeOpenGL2-8.1 \
-lvtkDomainsChemistryOpenGL2-8.1      -lvtkFiltersVerdict-8.1          -lvtkIOAMR-8.1                 -lvtkjsoncpp-8.1         -lvtksqlite-8.1 \
-lvtkexoIIc-8.1                       -lvtkfreetype-8.1                -lvtkIOCore-8.1                -lvtklibharu-8.1         -lvtksys-8.1 \
-lvtkexpat-8.1                        -lvtkGeovisCore-8.1              -lvtkIOEnSight-8.1             -lvtklibxml2-8.1         -lvtktiff-8.1 \
-lvtkFiltersAMR-8.1                   -lvtkgl2ps-8.1                   -lvtkIOExodus-8.1              -lvtklz4-8.1             -lvtkverdict-8.1 \
-lvtkFiltersCore-8.1                  -lvtkglew-8.1                    -lvtkIOExport-8.1              -lvtkmetaio-8.1          -lvtkViewsContext2D-8.1 \
-lvtkFiltersExtraction-8.1            -lvtkGUISupportQt-8.1            -lvtkIOExportOpenGL2-8.1       -lvtkNetCDF-8.1          -lvtkViewsCore-8.1 \
-lvtkFiltersFlowPaths-8.1             -lvtkGUISupportQtSQL-8.1         -lvtkIOGeometry-8.1            -lvtknetcdfcpp-8.1       -lvtkViewsInfovis-8.1 \
-lvtkFiltersGeneral-8.1               -lvtkhdf5-8.1                    -lvtkIOImage-8.1               -lvtkoggtheora-8.1       -lvtkViewsQt-8.1 \
-lvtkFiltersGeneric-8.1               -lvtkhdf5_hl-8.1                 -lvtkIOImport-8.1              -lvtkParallelCore-8.1    -lvtkzlib-8.1 \
-lvtkFiltersGeometry-8.1              -lvtkImagingColor-8.1            -lvtkIOInfovis-8.1             -lvtkpng-8.1 \
-lvtkFiltersHybrid-8.1                -lvtkImagingCore-8.1             -lvtkIOLegacy-8.1              -lvtkproj4-8.1 \
#-lpcl_common    -lpcl_io_ply  -lpcl_keypoints  -lpcl_outofcore    -lpcl_registration      -lpcl_segmentation  -lpcl_tracking \
#-lpcl_io      -lpcl_ml         -lpcl_people       -lpcl_sample_consensus  -lpcl_stereo        -lpcl_visualization \
#-lpcl_filters   -lpcl_kdtree  -lpcl_octree     -lpcl_recognition  -lpcl_search            -lpcl_surface \
#-lpcl_features -llz4 \
#-lG4analysis           -lG4global         -lG4particles     -lG4Tree \
#-lG4clhep              -lG4GMocren        -lG4persistency   -lG4visHepRep \
#-lG4digits_hits        -lG4graphics_reps  -lG4physicslists  -lG4vis_management \
#-lG4error_propagation  -lG4intercoms      -lG4processes     -lG4visXXX \
#-lG4event              -lG4interfaces     -lG4RayTracer     -lG4VRML \
#-lG4expat              -lG4materials      -lG4readout       -lG4zlib \
#-lG4FR                 -lG4modeling       -lG4run \
#-lG4geometry           -lG4OpenGL         -lG4tracking \
#-lG4gl2ps              -lG4parmodels      -lG4track \
#-L/home/jiping/FENGSim/FENGSim/GDT/build -lexample2

# QT += svg
#INCLUDEPATH += /usr/local/lib/R/library/RInside/include/ /usr/local/lib/R/library/Rcpp/include/ /usr/local/lib/R/include
#LIBS += -L/usr/local/lib/R/lib -lRlapack -lRblas -lR -L/usr/local/lib/R/library/RInside/lib/ -lRInside \
#-L/usr/local/lib/R/library/Rcpp/libs/Rcpp.so



#QMAKE_LFLAGS += -no-pie

DISTFILES += \
    figure/cad_wind/tree.png
