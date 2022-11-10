if(NOT DEFINED USER_PROGRAM)
  set(USER_PROGRAM
      "CAEStaticConsole"
      CACHE STRING "The user program's name")
endif()

add_executable(${USER_PROGRAM})
# message(STATUS "The user source files " ${USER_SRC} ${USER_PROGRAM}
#                ${OCP_SDK_SRC} ${OCP_SDK_LINK})

# message(STATUS "SDK SRC" ${OCP_SDK_SRC})

target_sources(${USER_PROGRAM} PRIVATE main.c ${OCP_SDK_SRC}
                                       ${USER_PROGRAM_SRC})
target_link_libraries(${USER_PROGRAM} PRIVATE ${OCP_SDK_LINK})
target_include_directories(${USER_PROGRAM} PRIVATE ${OCP_ROOT})


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
