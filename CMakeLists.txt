cmake_minimum_required(VERSION 3.16)
project(
  OCPSDK
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

add_subdirectory(framework/interface)
add_subdirectory(toolkit)
add_subdirectory(framework/lifecycle)
add_subdirectory(starter)
add_subdirectory(docs)
# option(OCP_Cli "Should Cli be built?" ON)
# if(OCP_Cli) 
  add_subdirectory(cli)
# endif()

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

# if(PROJECT_IS_TOP_LEVEL) cmake_minimum_required(VERSION 3.20)

# project(OpenCAXPlusframework) option(ENABLE_TEST "Enable testing" ON)
# if(ENABLE_TEST) include(CTest) endif() endif()
