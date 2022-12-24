#
include(OCPToolkitExternalBuildInstall)
ocp_toolkit_external_build_install(SOFTWARE OCE VERSION 0.18.3 FLAGS "-DOCE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install/OCE-0.18.3")


add_library(external_OCE INTERFACE IMPORTED GLOBAL)
add_library(OCP::external_OCE ALIAS external_OCE)