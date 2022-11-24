set(external_name spdlog)
set(version 1.11.0)
include(OCPToolkitExternal)
ocp_toolkit_external(SOFTWARE spdlog VERSION 1.11.0)

add_library(external_${external_name} INTERFACE IMPORTED GLOBAL)
add_library(OCP::External::${external_name} ALIAS external_${external_name})

set_property(
  TARGET external_${external_name}
  APPEND
  PROPERTY INTERFACE_LINK_LIBRARIES spdlog::spdlog)
