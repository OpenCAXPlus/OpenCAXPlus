set(SOFTWARE BLAS)
set(VERSION 3.11.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION}
                                   FLAGS "-DBUILD_SHARED_LIBS=ON")

if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)
  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES blas)

  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/include)
endif()
