cmake_minimum_required(VERSION 3.16)
project(
  OCP_INSTALL
  DESCRIPTION "The OpenCAXPlus SDK"
  VERSION 0.0.2
  LANGUAGES C CXX Fortran)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(OCP_ROOT
    /opt/ocp
    CACHE FILEPATH "The OpenCAX+SDK root path" FORCE)

set(OCP_CACHE
    $ENV{HOME}/ocp/cache
    CACHE FILEPATH "The OpenCAX+SDK cache path" FORCE)

cmake_host_system_information(RESULT Ncpu QUERY NUMBER_OF_PHYSICAL_CORES)

set(ncore
    ${Ncpu}
    CACHE STRING "Number of cores for make" FORCE)
add_subdirectory(framework/interface)
add_subdirectory(toolkit)
