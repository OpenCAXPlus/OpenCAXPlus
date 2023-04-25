cmake_minimum_required(VERSION 3.16)
project(
  OCP
  DESCRIPTION "The OpenCAXPlus SDK"
  VERSION 0.0.2
  LANGUAGES C CXX Fortran)

option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(OCP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(OCPPrepareVariables)

option(OCP_SDK_INSIDE "Inside OCP SDK" ON)

add_subdirectory(cli)

add_subdirectory(framework/interface)

# ##############################################################################
# 1. go to the directory for toolkit
#
# 1. go to the directory for TPS, for example M++
#
# 1. install_all is the total target, you need to understant target and command
#   in cmake
#
# 1. install_* is the target to compile one TPS, for example M++
#
# 1. check if the directory "/home/username/OCP1.0/cache/M++/" exists, if it is
#   not, go to the directory OCP/toolkit/M++/external
#
# 1. uncompress the source tar.xz and use bash commands to compile, pay
#   attentation to the dependency from target to command (compile) to command
#   (uncompress)
#
# 1. there are install_target, external_target and too_target external _target
#   and tool_target are empty now
# ##############################################################################

add_subdirectory(toolkit)

add_subdirectory(framework/lifecycle)
add_subdirectory(starter)
add_subdirectory(docs)

cmake_host_system_information(RESULT Ncpu QUERY NUMBER_OF_PHYSICAL_CORES)
message(STATUS "CMake ${CMAKE_VERSION} using ${Ncpu} threads")

set(ncore
    ${Ncpu}
    CACHE STRING "Number of cores for make" FORCE)
include(CPackConfig)
# include(GNUInstallDirs)

# include(CMakePackageConfigHelpers) write_basic_package_version_file(
# "OpenCAXPlusSDKConfigVersion.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY
# SameMajorVersion)

install(
  DIRECTORY framework/interface
  DESTINATION framework
  COMPONENT ${PROJECT_NAME})

install(
  DIRECTORY framework/lifecycle
  DESTINATION framework
  COMPONENT ${PROJECT_NAME})

install(
  DIRECTORY cmake
  DESTINATION .
  COMPONENT ${PROJECT_NAME})

install(
  FILES scripts/CMakePresets.json scripts/CMakeLists.txt scripts/setvars.sh
  DESTINATION .
  COMPONENT ${PROJECT_NAME})
# if(PROJECT_IS_TOP_LEVEL) cmake_minimum_required(VERSION 3.20)

# project(OpenCAXPlusframework) option(ENABLE_TEST "Enable testing" ON)
# if(ENABLE_TEST) include(CTest) endif() endif()
