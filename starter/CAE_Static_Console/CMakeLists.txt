cmake_minimum_required(VERSION 3.20)

project(CAESCStarter)
option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
message(STATUS "Cmake module path " ${CMAKE_MODULE_PATH})
include(OCPEmbedUserProgram)

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
  "${CMAKE_CURRENT_SOURCE_DIR}/../.."
  USE_CXX)
