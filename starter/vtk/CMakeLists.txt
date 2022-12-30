cmake_minimum_required(VERSION 3.20)

project(vtk_starter)

set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK)

add_library(vtkStarter STATIC)
target_sources(vtkStarter PUBLIC app.cpp define.cpp)

ocp_embed_user_program(
  LIFECYCLE
  "basic_vtk"
  TOOLS
  "spdlog;VTK"
  NAME
  "vtk_starter"
  TARGET
  vtkStarter
  ROOT
  "${OCP_ROOT}"
  USE_CXX)


# target_link_libraries(vtk_starter PRIVATE ${VTK_LIBRARIES})
# include(${VTK_USE_FILE})
