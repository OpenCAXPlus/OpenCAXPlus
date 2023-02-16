set(SOFTWARE spdlog)
set(VERSION 1.11.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION})

if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)
  set(spdlog_DIR ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/lib/cmake/spdlog)
  find_package(spdlog)
  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES spdlog::spdlog)
endif()
