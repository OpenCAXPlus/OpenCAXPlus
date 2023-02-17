set(SOFTWARE lapack)
set(VERSION 3.11.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION})

if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)
  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES lapack)

  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/include)
endif()
