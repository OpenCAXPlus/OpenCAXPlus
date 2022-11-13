cmake_minimum_required(VERSION 3.20)

project(OpenCAXPlusStarter)
option(ENABLE_TEST "Enable testing" ON)
if(ENABLE_TEST)
  include(CTest)
endif()


add_subdirectory(CAE_Static_Console)
