cmake_minimum_required(VERSION 3.21)

project(
  OpenCAXPlusSDK
  DESCRIPTION "The OpenCAXPlus SDK"
  LANGUAGES C CXX Fortran)

option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE FILEPATH "The OpenCAX+SDK root path")

set(OCP_SDK_SRC
    ""
    CACHE INTERNAL "")
set(OCP_SDK_LINK
    ""
    CACHE INTERNAL "")
set(USER_SRC_PATH
    "starter"
    CACHE FILEPATH "The location where user defined his/her own functions")

add_subdirectory(toolkit)
add_subdirectory(framework/interface)

add_subdirectory(${USER_SRC_PATH})

add_subdirectory(framework/lifecycle)
