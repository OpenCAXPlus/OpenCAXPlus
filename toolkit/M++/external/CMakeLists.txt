set(SOFTWARE M++)
set(VERSION 1.8.0)

include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION})

if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)
  find_package(MPI)
  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES M++ MPI::MPI_CXX lapack blas superlu)

  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_DIRECTORIES
             ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/lib)

  set_property(
    TARGET external_${SOFTWARE}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/include)
endif()
