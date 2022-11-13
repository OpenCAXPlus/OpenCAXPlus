add_library(solver)
add_library(OCP::Toolkit::Solver ALIAS solver)

message(STATUS "The OCP backend solver library " ${OCP_BACKEND_SOLVER})
add_subdirectory(${OCP_BACKEND_SOLVER})

# if(NOT ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") AND (${CMAKE_CXX_COMPILER_ID}
# STREQUAL "Intel"))) add_subdirectory(spdlog) endif()
