cmake_minimum_required(VERSION 3.20)

project(BasicConsoleStarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)

add_library(CAEStatic STATIC)
target_sources(CAEStatic PUBLIC App.cpp)

ocp_embed_user_program(
  LIFECYCLE
  "Console"
  TOOLS
  "spdlog;faspsolver"
  NAME
  "BasicConsole"
  TARGET
  CAEStatic
  ROOT
  ${OCP_ROOT}
  USE_CXX)
