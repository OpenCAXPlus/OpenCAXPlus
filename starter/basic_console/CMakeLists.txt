cmake_minimum_required(VERSION 3.20)

project(BasicConsoleStarter)

set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK)

add_library(CAEStatic STATIC)
target_sources(CAEStatic PUBLIC app.cpp define.cpp)
# target_include_directories(CAEStatic PUBLIC ${OCP_ROOT}/toolkit
# ${OCP_ROOT}/framework/lifecycle/BasicConsole)

ocp_embed_user_program(
  LIFECYCLE
  "basic_console"
  TOOLS
  "spdlog;faspsolver"
  NAME
  "basic_console"
  TARGET
  CAEStatic
  ROOT
  ${OCP_ROOT}
  USE_CXX)
