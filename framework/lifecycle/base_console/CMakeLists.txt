add_executable(${StarterName})
target_link_libraries(${StarterName} PUBLIC OCP::Interface)
target_sources(${StarterName} PUBLIC main.c define.h)

# Add any necessary linked library
add_test(
  NAME ${StarterName}_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${StarterName})
