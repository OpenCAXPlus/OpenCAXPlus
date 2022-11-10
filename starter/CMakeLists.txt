if(PROJECT_IS_TOP_LEVEL)
  cmake_minimum_required(VERSION 3.20)

  project(OpenCAXPlusStarter)
  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()

  set(OCP_LIFECYCLE
      "CAE_Static_Console"
      CACHE STRING "The program life cycle that user want to use")

endif()

message(STATUS ${OCP_LIFECYCLE})

add_subdirectory(${OCP_LIFECYCLE})
