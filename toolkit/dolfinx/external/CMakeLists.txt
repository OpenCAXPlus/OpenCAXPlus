set(SOFTWARE dolfinx)
set(VERSION 0.6.0)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  FLAGS
  "-Dpugixml_DIR=${OCP_CACHE}/pugixml/1.13/install/lib/cmake/pugixml"
  "-DBasix_DIR=${OCP_CACHE}/basix/0.6.0/install/lib/cmake/basix"
  "-DHDF5_DIR=${OCP_CACHE}/hdf5/1.14.0/install/cmake"
  "-DUFCx_DIR=${OCP_CACHE}/ffcx/0.6.0/install/share/ufcx/cmake"
  "-DMPI_HOME=${OCP_CACHE}/petsc/3.18.4/install"
  SUBFOLDER
  cpp
  DEPENDS
  "dolfinx_virtual_file"
  ENVS
  "PETSC_DIR=${OCP_CACHE}/petsc/3.18.4/install"
  "PETSC_ARCH=''")

add_custom_command(
  OUTPUT dolfinx_virtual_file
  DEPENDS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install
  WORKING_DIRECTORY ${OCP_CACHE}/${SOFTWARE}/${VERSION}/source/python
  COMMAND
    ${CMAKE_COMMAND} -E env
    PETSC_DIR=${OCP_CACHE}/petsc/3.18.4/install;PETSC_ARCH=;PYTHONPATH=${OCP_CACHE}/petsc/3.18.4/install/lib;CMAKE_ARGS=-DDOLFINX_DIR=${OCP_CACHE}/dolfinx/0.6.0/install/lib/cmake/dolfinx\ -Dpugixml_DIR=${OCP_CACHE}/pugixml/1.13/install/lib/cmake/pugixml\ -DHDF5_DIR=${OCP_CACHE}/hdf5/1.14.0/install/cmake\ -DMPI_C_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpicc\ -DMPI_CXX_COMPILER=${OCP_CACHE}/petsc/3.18.4/install/bin/mpic++
    pip install .
  VERBATIM USES_TERMINAL
  COMMENT "Pip Install dolfinx")

add_dependencies(install_dolfinx install_pugixml install_petsc install_hdf5
                 install_basix install_ffcx)
