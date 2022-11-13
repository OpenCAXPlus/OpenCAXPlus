cmake_minimum_required(VERSION 3.20)

project(CAESCStarter)
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
    "spdlog"
    CACHE INTERNAL "")

set(USER_PROGRAM
    CAEStatic
    CACHE INTERNAL "")

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    CACHE INTERNAL "")

set(USER_PROGRAM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/InputRead.cpp
    CACHE INTERNAL "")

set(USER_CXX
    ON
    CACHE BOOL "" FORCE)

add_subdirectory(${OCP_ROOT}/toolkit ${CMAKE_CURRENT_BINARY_DIR}/toolkit)

add_subdirectory(${OCP_ROOT}/framework/lifecycle
                 ${CMAKE_CURRENT_BINARY_DIR}/framework/lifecycle)
