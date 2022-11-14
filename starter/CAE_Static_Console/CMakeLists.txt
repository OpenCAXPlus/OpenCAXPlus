cmake_minimum_required(VERSION 3.20)

project(CAESCStarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)


add_library(CAEStatic STATIC)
target_sources(CAEStatic PUBLIC All.cpp)

ocp_embed_user_program(
  LIFECYCLE
  "CAE_Static_Console"
  LOG
  "spdlog"
  SOLVER
  "faspsolver"
  NAME
  "CAE_Static_Console"
  TARGET
  CAEStatic
  ROOT
  ${OCP_ROOT}
  USE_CXX)
