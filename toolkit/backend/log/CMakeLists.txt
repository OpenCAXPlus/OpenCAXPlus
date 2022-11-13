add_library(log)
add_library(OCP::Toolkit::Log ALIAS log)

add_subdirectory(default)

# if(NOT ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") AND (${CMAKE_CXX_COMPILER_ID}
# STREQUAL "Intel"))) add_subdirectory(spdlog) endif()
