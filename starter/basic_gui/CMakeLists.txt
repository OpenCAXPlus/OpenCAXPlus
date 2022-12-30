cmake_minimum_required(VERSION 3.20)

project(
  helloworld)
# cmake_policy(SET CMP0076 NEW)
set(OCPSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OCPSDK)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_DIR ${OCP_ROOT}/toolkit/Qt/external/install/qtbase-5.15.2/lib/cmake/Qt5)

find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)

add_library(helloworld)
target_sources(helloworld PUBLIC app.cpp define.cpp)
target_link_libraries(helloworld PUBLIC Qt5::Widgets)

ocp_embed_user_program(
  LIFECYCLE
  "basic"
  TOOLS
  "spdlog"
  NAME
  "basic"
  TARGET
  helloworld
  ROOT
  ${OCP_ROOT}
  USE_CXX)
