cmake_minimum_required(VERSION 3.21)

project(OpenCAXPlusFramework)

if(PROJECT_IS_TOP_LEVEL)
  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()
endif()



add_subdirectory(interface)

add_subdirectory(lifecycle)
