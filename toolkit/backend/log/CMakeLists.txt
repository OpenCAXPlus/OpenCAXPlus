add_library(log INTERFACE IMPORTED GLOBAL)
add_library(OCP::Toolkit::Log ALIAS log)

message(STATUS "The OCP backend logging library " ${OCP_BACKEND_LOG})
set_property(
  TARGET log
  APPEND
  PROPERTY INTERFACE_LINK_LIBRARIES OCP::Toolkit::Log::${OCP_BACKEND_LOG})
  # if(NOT ((${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") AND (${CMAKE_CXX_COMPILER_ID}
# STREQUAL "Intel"))) add_subdirectory(spdlog) endif()

add_subdirectory(default)
add_subdirectory(spdlog)
