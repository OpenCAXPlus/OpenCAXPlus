cmake_minimum_required(VERSION 3.20)

project(BasicConsoleStarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)

add_library(CAEStatic STATIC)
target_sources(CAEStatic PUBLIC App.cpp Define.cpp)
# target_include_directories(CAEStatic PUBLIC ${OCP_ROOT}/toolkit
#                                             ${OCP_ROOT}/framework/lifecycle/BasicConsole)

ocp_embed_user_program(
  LIFECYCLE
  "BasicConsole"
  TOOLS
  "spdlog;faspsolver"
  NAME
  "BasicConsole"
  TARGET
  CAEStatic
  ROOT
  ${OCP_ROOT}
  USE_CXX)
