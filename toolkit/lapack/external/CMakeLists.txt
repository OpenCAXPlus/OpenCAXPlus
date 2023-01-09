#
include(OCPToolkitExternalBuildInstall)

ocp_toolkit_external_build_install(SOFTWARE lapack VERSION 3.11.0 FLAGS "-DBUILD_SHARED_LIBS=ON")

add_library(external_lapack INTERFACE IMPORTED GLOBAL)
add_library(OCP::external_lapack ALIAS external_lapack)
