cmake_minimum_required(VERSION 3.20)

project(FASPStarter)


set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)


add_library(faspstarter STATIC)
target_sources(faspstarter PUBLIC fasp.c)

ocp_embed_user_program(
  LIFECYCLE
  "CAE_Static_Console"
  LOG
  "default"
  SOLVER
  "faspsolver"
  NAME
  "ocpfaspstarter"
  TARGET
  faspstarter
  ROOT
  "${OCP_ROOT}")
