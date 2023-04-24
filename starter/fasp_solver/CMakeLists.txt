cmake_minimum_required(VERSION 3.20)

project(FASPStarter)

set(OCP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCP)

add_library(faspstarter STATIC)
target_sources(faspstarter PUBLIC app.c app.h define.c)
# target_include_directories(faspstarter PUBLIC ${OCP_ROOT}/toolkit
#                                             ${OCP_ROOT}/framework/lifecycle/BasicConsole)

ocp_embed_user_program(
  LIFECYCLE
  "basic"
  TOOLS
  "ocplog;faspsolver"
  NAME
  "fasp_solver"
  TARGET
  faspstarter
  ROOT
  "${OCP_ROOT}"
  COPYFILES
  input.dat)
