cmake_minimum_required(VERSION 3.20)

project(FASPStarter)
option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
message(STATUS "Cmake module path " ${CMAKE_MODULE_PATH})
include(PrepareVariables)

set(OCP_LIFECYCLE
    "CAE_Static_Console"
    CACHE INTERNAL "")

set(OCP_BACKEND_LOG
    "default"
    CACHE INTERNAL "")

set(OCP_BACKEND_SOLVER
    "faspsolver"
    CACHE INTERNAL "")

set(USER_PROGRAM
    ocpfaspstarter
    CACHE INTERNAL "")

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    CACHE INTERNAL "")

set(USER_PROGRAM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fasp.c
    CACHE INTERNAL "")

set(USER_CXX
    OFF
    CACHE BOOL "" FORCE)

add_subdirectory(${OCP_ROOT}/toolkit ${PROJECT_BINARY_DIR}/toolkit)

add_subdirectory(${OCP_ROOT}/framework/lifecycle
                 ${PROJECT_BINARY_DIR}/framework/lifecycle)
