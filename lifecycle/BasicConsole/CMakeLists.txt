add_executable(${USER_PROGRAM})
target_link_libraries(${USER_PROGRAM} PUBLIC OCP::Interface)
target_include_directories(${USER_PROGRAM} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${USER_PROGRAM} PUBLIC main.c)

if(OCP_SDK_INSIDE)
  target_sources(${USER_PROGRAM} PUBLIC dummy.c)
  foreach(target ${OCP_TOOLS})
    set_property(
      TARGET ${USER_PROGRAM}
      APPEND
      PROPERTY LINK_LIBRARIES OCP::Toolkit::${target})
  endforeach()
else()
  target_link_libraries(${USER_PROGRAM} PUBLIC ${USER_PROGRAM_TARGET})
endif()

if(ENABLE_TEST)
  add_test(
    NAME ${USER_PROGRAM}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${USER_PROGRAM})
endif()
