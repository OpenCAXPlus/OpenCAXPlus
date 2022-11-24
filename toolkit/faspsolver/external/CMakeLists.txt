set(external_name faspsolver)
set(version 2.2.0)
include(OCPToolkitExternal)
ocp_toolkit_external(SOFTWARE ${external_name} VERSION ${version})

add_library(external_${external_name} INTERFACE IMPORTED GLOBAL)
add_library(OCP::External::${external_name} ALIAS external_${external_name})

set_property(
  TARGET external_${external_name}
  APPEND
  PROPERTY INTERFACE_LINK_LIBRARIES fasp m)

set_property(
  TARGET external_${external_name}
  APPEND
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES
           ${CMAKE_CURRENT_SOURCE_DIR}/${external_name}-${version}/base/include)
