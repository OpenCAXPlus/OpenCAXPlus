cmake_minimum_required(VERSION 3.16)
project(
  ocpsdk
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
include(GNUInstallDirs)

option(OCP_SDK_INSIDE "Inside OCP SDK" ON)

add_subdirectory(framework/interface)

# add_subdirectory(external)
# add_subdirectory(toolkit)
# add_subdirectory(framework/lifecycle)
# add_subdirectory(starter)
# add_subdirectory(docs)
cmake_host_system_information(RESULT Ncpu QUERY NUMBER_OF_PHYSICAL_CORES)
message(STATUS "CMake ${CMAKE_VERSION} using ${Ncpu} threads")

set(ncore
    ${Ncpu}
    CACHE STRING "Number of cores for make" FORCE)
include(CPackConfig)

install(
  DIRECTORY framework/lifecycle
  DESTINATION src
  COMPONENT ${PROJECT_NAME})

install(
  DIRECTORY cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
  COMPONENT ${PROJECT_NAME})

install(
  FILES scripts/CMakePresets.json scripts/CMakeLists.txt
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
  COMPONENT ${PROJECT_NAME})

install(
  FILES scripts/setvars.sh
  DESTINATION .
  COMPONENT ${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/OCPConfig.cmake.in
  "OCPConfig.cmake"
    INSTALL_DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/ocp)

write_basic_package_version_file(
    OCPConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
  "${CMAKE_CURRENT_BINARY_DIR}/OCPConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/OCPConfigVersion.cmake"
    DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/ocp)
