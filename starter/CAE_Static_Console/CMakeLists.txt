cmake_minimum_required(VERSION 3.20)

project(CAESCStarter)
option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()

set(OCP_LIFECYCLE
    "CAE_Static_Console"
    CACHE STRING "The program life cycle that user want to use")

set(USER_PROGRAM
    CAEStatic
    CACHE STRING "The user source program" FORCE)

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    CACHE FILEPATH "The OpenCAX+SDK root path" FORCE)
message(STATUS "source dir" ${CMAKE_CURRENT_SOURCE_DIR} | ${OCP_ROOT})

add_subdirectory(${OCP_ROOT}/framework/interface
                 ${CMAKE_CURRENT_BINARY_DIR}/framework/interface)
add_subdirectory(${OCP_ROOT}/toolkit ${CMAKE_CURRENT_BINARY_DIR}/toolkit)

set(USER_PROGRAM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/InputRead.cpp
    CACHE INTERNAL "")

set(USER_CXX
    ON
    CACHE BOOL "" FORCE)

add_subdirectory(${OCP_ROOT}/framework/lifecycle
                 ${CMAKE_CURRENT_BINARY_DIR}/framework/lifecycle)
