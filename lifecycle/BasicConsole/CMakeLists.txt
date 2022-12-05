add_executable(${USER_PROGRAM})

if(OCP_SDK_INSIDE)
  # set(USER_PROGRAM_SRC dummy.c) set(USER_INCLUDE ${OCP_ROOT})
  # add_library(${USER_PROGRAM} STATIC)
  target_sources(${USER_PROGRAM} PUBLIC main.c dummy.c)
  foreach(target ${OCP_TOOLS})
    message(STATUS "Adding tool " ${target})
    # add_subdirectory(${OCP_ROOT}/toolkit/${target})
    set_property(
      TARGET ${OCPUser_TARGET}
      APPEND
      PROPERTY INTERFACE_LINK_LIBRARIES OCP::Toolkit::${target})
  endforeach()
  target_link_libraries(${USER_PROGRAM} PUBLIC OCP::Interface)
  target_include_directories(${USER_PROGRAM}
                             PUBLIC ${OCP_ROOT}/lifecycle/${OCP_LIFECYCLE})
endif()

if(ENABLE_TEST)
  add_test(
    NAME ${USER_PROGRAM}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${USER_PROGRAM})
endif()
