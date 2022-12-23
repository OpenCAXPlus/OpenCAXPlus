cmake_minimum_required(VERSION 3.20)

project(vtk_starter)

set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK)





add_library(vtkStarter STATIC)
target_sources(vtkStarter PUBLIC app.cpp define.cpp)


# set(VTK_DIR
# ${OCP_ROOT}/toolkit/VTK/external/install/VTK-8.2.0/lib/cmake/vtk-8.2
# )
# find_package(VTK REQUIRED)
# vtk_module_config(VTK vtkCommonCore vtkFiltersSources vtkFiltersSources
#               vtkInteractionStyle vtkRenderingOpenGL2)
# target_link_libraries(vtkStarter PUBLIC ${VTK_LIBRARIES})

ocp_embed_user_program(
  LIFECYCLE
  "basic_console"
  TOOLS
  "spdlog;VTK"
  NAME
  "vtk_starter"
  TARGET
  vtkStarter
  ROOT
  "${OCP_ROOT}"
  USE_CXX)
