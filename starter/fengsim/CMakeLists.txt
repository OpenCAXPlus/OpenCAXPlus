cmake_minimum_required(VERSION 3.16.0)

project(
  FengSim
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK)
set(Qt5_DIR ${OCP_ROOT}/toolkit/Qt/external/install/5.12.10/lib/cmake/Qt5)

find_package(
  Qt5
  COMPONENTS Widgets PrintSupport
  REQUIRED)

set(OCE_DIR ${OCP_ROOT}/toolkit/OCE/external/install/OCE-0.18.3/lib/oce-0.18)
find_package(OCE REQUIRED)

set(gmsh_DIR ${OCP_ROOT}/toolkit/gmsh/external/install/gmsh-3.0.6)
set(gmsh_INCLUDE_DIRS ${gmsh_DIR}/include/gmsh)
set(gmsh_LIBRARIES -lGmsh)
# find_package(gmsh)

set(VTK_DIR ${OCP_ROOT}/toolkit/VTK/external/install/VTK-8.1.0/lib/cmake/vtk-8.1)
find_package(VTK REQUIRED)
vtk_module_config(
  VTK
  vtkCommonCore
  vtkIOImage
  vtkIOXML
  vtkInteractionWidgets
  vtkViewsQt
  vtkIOGeometry
  vtkFiltersPoints
  vtkRenderingVolume${VTK_RENDERING_BACKEND})
include(${VTK_USE_FILE})

find_package(lapack)

add_executable(FengSim)

target_include_directories(
  FengSim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OCE_INCLUDE_DIRS}
                  ${gmsh_INCLUDE_DIRS})
add_subdirectory(AdditiveManufacturing)
add_subdirectory(CAD)
add_subdirectory(FEM)
add_subdirectory(Machining)
add_subdirectory(Measure)
add_subdirectory(Mesh)
add_subdirectory(StatisticalProcessControl)
add_subdirectory(System)
add_subdirectory(Transport)
add_subdirectory(Visual)
target_sources(
  FengSim
  PRIVATE Main.cpp
          MainWindow.cpp
          qcustomplot.cpp
          AboutDialog.cpp
          MainWindow.qrc
          MeshWindow.qrc
          FEMWindow.qrc
          CADWindow.qrc
          DataBaseWindow.qrc
          MeasureWidnow.qrc
          System.qrc
          SPCWindow.qrc
          AMWindow.qrc
          MainWindow.ui
          AboutDialog.ui)

target_link_directories(FengSim PRIVATE ${gmsh_DIR}/lib)
target_link_libraries(
  FengSim
  Qt5::Widgets
  Qt5::PrintSupport
  Qt5::Core
  Qt5::Gui
  ${OCE_LIBRARIES}
  ${VTK_LIBRARIES}
  ${gmsh_LIBRARIES})
