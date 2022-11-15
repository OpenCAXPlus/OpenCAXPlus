cmake_minimum_required(VERSION 3.20)

project(
  OpenCAXPlusSDK
  DESCRIPTION "The OpenCAXPlus SDK"
  VERSION 0.0.1
  LANGUAGES C CXX Fortran)

option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(OCPPrepareVariables)

option(OCP_SDK_INSIDE "Inside OCP SDK" ON)

# set(USER_SRC_PATH "starter" CACHE FILEPATH "The location where user defined
# his/her own functions")

add_subdirectory(toolkit)
add_subdirectory(framework/lifecycle)
add_subdirectory(starter)

include(CPackConfig)

install(DIRECTORY framework DESTINATION .)
