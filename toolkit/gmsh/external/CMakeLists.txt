set(SOFTWARE gmsh)
set(VERSION 4.11.1)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  FLAGS
  "-DCMAKE_BUILD_TYPE=Release"
  "-DDEFAULT=0"
  "-DENABLE_BUILD_LIB=0"
  "-DENABLE_POST=1"
  "-DENABLE_PARSER=1"
  "-DENABLE_BUILD_SHARED=1"
  "-DENABLE_BUILD_DYNAMIC=1"
  "-DCMAKE_LIBRARY_PATH=${OCP_CACHE}/lapack/3.11.0/install"
  "-DCMAKE_C_FLAGS=-lgfortran")

if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)
  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES gmsh)

  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/include)
endif()

add_dependencies(install_gmsh install_lapack)
