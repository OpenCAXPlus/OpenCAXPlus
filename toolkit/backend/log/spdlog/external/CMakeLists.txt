# include(FetchContent)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
  execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xz" "spdlog-1.11.0.tar.gz"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog) message(STATUS "spdlog exist")
add_subdirectory(spdlog-1.11.0) # version 1.11.0
# endif()
