if(OCP_SDK_INSIDE)
  # set(USER_PROGRAM_SRC dummy.c) set(USER_INCLUDE ${OCP_ROOT})
  add_library(user_target STATIC)
  target_sources(user_target PUBLIC dummy.c)
  target_link_libraries(user_target PUBLIC OCP::Toolkit::Log
                                           OCP::Toolkit::Solver)
  target_include_directories(
    user_target PUBLIC ${OCP_ROOT}
                       ${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE})
  set(USER_PROGRAM_TARGET user_target)
endif()

add_executable(${USER_PROGRAM})
target_sources(${USER_PROGRAM} PRIVATE main.c global.h)
target_link_libraries(${USER_PROGRAM} PRIVATE ${USER_PROGRAM_TARGET})
# target_include_directories( ${USER_PROGRAM} PRIVATE
# ${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE})

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
