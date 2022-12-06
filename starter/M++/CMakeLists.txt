cmake_minimum_required(VERSION 3.20)

project(mppstarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)
# find_package(MPI REQUIRED)

add_library(mppstarter STATIC)
target_sources(mppstarter PUBLIC src/App.cpp src/Poisson.cpp
                                 src/PoissonProblems.cpp src/Define.cpp)
target_include_directories(mppstarter PUBLIC ${OCP_ROOT}/toolkit
                                             ${OCP_ROOT}/lifecycle/BasicConsole)
# add_compile_options(-diag-disable=10441)
ocp_embed_user_program(
  LIFECYCLE
  "BasicConsole"
  TOOLS
  "spdlog;M++"
  NAME
  "Mpp"
  TARGET
  mppstarter
  ROOT
  "${OCP_ROOT}"
  COPYFILES
  conf/poisson.conf
  USE_CXX)
