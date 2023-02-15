set(SOFTWARE ffcx)
set(VERSION 0.6.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  SUBFOLDER
  cmake
  DEPENDS
  "~/.local/bin/ffcx")

# add_custom_target(pip_${SOFTWARE} DEPENDS ~/.local/bin/ffcx)
add_custom_command(
  OUTPUT ~/.local/bin/ffcx
  DEPENDS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/source
  WORKING_DIRECTORY ${OCP_CACHE}/${SOFTWARE}/${VERSION}/source
  COMMAND pip install .
  VERBATIM USES_TERMINAL
  COMMENT "Install FFCx")
