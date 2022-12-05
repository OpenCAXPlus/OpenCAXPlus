set(external_name faspsolver)
set(version 2.2.0)
include(OCPToolkitExternal)

ocp_toolkit_external(
  SOFTWARE
  ${external_name}
  VERSION
  ${version}
  LIBRARIES
  fasp
  m
  INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/${external_name}-${version}/base/include)
