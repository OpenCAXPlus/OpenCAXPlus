set(SOFTWARE M++)
set(VERSION 1.8.0)

include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  FLAGS
  "-DMPI_C_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpicc"
  "-DMPI_CXX_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpic++"
  "-Dsuperlu_DIR=${OCP_CACHE}/petsc/3.18.4/install/lib/cmake/superlu")

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

add_dependencies(install_M++ install_petsc install_lapack)
