add_library(log)
add_library(OCP::Toolkit::Log ALIAS log)

message(STATUS "The OCP backend logging library " ${OCP_BACKEND_LOG})
add_subdirectory(${OCP_BACKEND_LOG})

# if(NOT ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") AND (${CMAKE_CXX_COMPILER_ID}
# STREQUAL "Intel"))) add_subdirectory(spdlog) endif()
