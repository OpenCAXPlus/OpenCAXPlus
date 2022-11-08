if(PROJECT_IS_TOP_LEVEL)
  cmake_minimum_required(VERSION 3.21)

  project(CAESCStarter)
  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()

  set(OCP_LIFECYCLE
      "CAE_Static_Console"
      CACHE STRING "The program life cycle that user want to use")

  set(USER_PROGRAM
      testprogram
      CACHE STRING "The user source program" FORCE)
  set(OCP_ROOT
      "somepath"
      CACHE FILEPATH "The OpenCAX+SDK root path")
  add_subdirectory(${OCP_ROOT}/framework/interface)
  add_subdirectory(${OCP_ROOT}/toolkit)
endif()

message(STATUS "The user program " ${USER_PROGRAM})
# target_sources(${USER_PROGRAM} PUBLIC InputRead.cpp)
set(USER_PROGRAM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/InputRead.cpp
    CACHE INTERNAL "")

set(USER_CXX
    ON
    CACHE BOOL "" FORCE)

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(${OCP_ROOT}/framework/lifecycle)
endif()
