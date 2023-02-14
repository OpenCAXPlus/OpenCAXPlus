cmake_minimum_required(VERSION 3.20)

project(oce_starter)

set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK REQUIRED)

add_library(oceStarter STATIC)
target_sources(oceStarter PUBLIC app.cpp define.cpp)

set(OCE_DIR ${OCP_CACHE}/OCE/0.18.3/install/lib/oce-0.18)
find_package(OCE REQUIRED COMPONENTS TKPrim)

# Include files reside in ${OCE_INCLUDE_DIRS};
target_include_directories(oceStarter PUBLIC ${OCE_INCLUDE_DIRS})

# We do not need library path, they will be automatically imported.
target_link_libraries(oceStarter PUBLIC TKPrim)

ocp_embed_user_program(
  LIFECYCLE
  "basic"
  TOOLS
  "spdlog;OCE"
  NAME
  "oce_starter"
  TARGET
  oceStarter
  ROOT
  "${OCP_ROOT}"
  USE_CXX)

# target_link_libraries(vtk_starter PRIVATE ${VTK_LIBRARIES})
# include(${VTK_USE_FILE})
