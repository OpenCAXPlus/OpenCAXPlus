set(SOFTWARE hdf5)
set(VERSION 1.14.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  FLAGS
  "-DHDF5_ENABLE_PARALLEL=ON"
  "-DMPI_C_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpicc"
  "-DMPI_CXX_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpic++")
