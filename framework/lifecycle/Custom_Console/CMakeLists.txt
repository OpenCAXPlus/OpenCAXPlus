add_executable(${USER_PROGRAM})

if(OCP_SDK_INSIDE)
  # set(USER_PROGRAM_SRC dummy.c) set(USER_INCLUDE ${OCP_ROOT})
  add_library(${USER_PROGRAM} STATIC)
  target_sources(${USER_PROGRAM} PUBLIC main.c)
  target_link_libraries(${USER_PROGRAM} PUBLIC OCP::Toolkit::Log
                                           OCP::Toolkit::Solver)
  target_include_directories(
    ${USER_PROGRAM} PUBLIC ${OCP_ROOT}
                       ${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE})
endif()



if(ENABLE_TEST)
  add_test(
    NAME ${USER_PROGRAM}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${USER_PROGRAM})
endif()
