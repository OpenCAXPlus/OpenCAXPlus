if(NOT DEFINED USER_PROGRAM)
  set(USER_PROGRAM
      "CAEStaticConsole"
      CACHE STRING "The user program's name")
endif()

if(OCP_SDK_INSIDE)
  set(USER_PROGRAM_SRC dummy.c)
  set(USER_INCLUDE ${OCP_ROOT})
else()
  set(USER_INCLUDE ${OCP_ROOT}/toolkit)
endif()

add_executable(${USER_PROGRAM})
target_sources(${USER_PROGRAM} PRIVATE main.c ${USER_PROGRAM_SRC})
target_link_libraries(${USER_PROGRAM} PRIVATE OCP::Toolkit::Log)
target_include_directories(${USER_PROGRAM} PRIVATE ${USER_INCLUDE})

# message(STATUS "CXX " ${USER_CXX})
if(USER_CXX)
  set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
endif()

if(ENABLE_TEST)
  add_test(
    NAME ${USER_PROGRAM}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${USER_PROGRAM})
endif()
