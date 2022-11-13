cmake_minimum_required(VERSION 3.20)

project(FASPStarter)
option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
message(STATUS "Cmake module path " ${CMAKE_MODULE_PATH})
include(OCPEmbedUserProgram)

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
  "${CMAKE_CURRENT_SOURCE_DIR}/../..")
