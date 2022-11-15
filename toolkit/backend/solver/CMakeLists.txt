add_library(solver INTERFACE IMPORTED GLOBAL)
add_library(OCP::Toolkit::Solver ALIAS solver)

message(STATUS "The OCP backend solver library " ${OCP_BACKEND_SOLVER})

set_property(
  TARGET solver
  APPEND
  PROPERTY INTERFACE_LINK_LIBRARIES OCP::Toolkit::Solver::${OCP_BACKEND_SOLVER})

add_subdirectory(faspsolver)

# if(NOT ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") AND (${CMAKE_CXX_COMPILER_ID}
# STREQUAL "Intel"))) add_subdirectory(spdlog) endif()
