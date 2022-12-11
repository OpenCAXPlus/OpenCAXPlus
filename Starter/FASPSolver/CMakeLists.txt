cmake_minimum_required(VERSION 3.20)

project(FASPStarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)

add_library(faspstarter STATIC)
target_sources(faspstarter PUBLIC App.c App.h Define.c)
# target_include_directories(faspstarter PUBLIC ${OCP_ROOT}/toolkit
#                                             ${OCP_ROOT}/framework/lifecycle/BasicConsole)

ocp_embed_user_program(
  LIFECYCLE
  "BasicConsole"
  TOOLS
  "ocplog;faspsolver"
  NAME
  "FASPSolver"
  TARGET
  faspstarter
  ROOT
  "${OCP_ROOT}"
  COPYFILES
  input.dat)
