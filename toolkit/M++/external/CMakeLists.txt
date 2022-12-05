set(external_name M++)
set(version 1.8.0)
include(OCPToolkitExternal)
ocp_toolkit_external(SOFTWARE M++ VERSION 1.8.0)

add_library(external_${external_name} INTERFACE IMPORTED GLOBAL)
add_library(OCP::External::${external_name} ALIAS external_${external_name})

set_property(
  TARGET external_${external_name}
  APPEND
  PROPERTY INTERFACE_LINK_LIBRARIES M++)

# set_property(
#   TARGET external_${external_name}
#   APPEND
#   PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${external_name}-${version}/src)
