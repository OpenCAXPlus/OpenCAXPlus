set(SOFTWARE petsc)
set(VERSION 3.18.4)
string(REPLACE "." "" VERSION_SHORT ${VERSION})
include(OCPCompress)

set(rootpath ${OCP_CACHE}/${SOFTWARE}/${VERSION})
set(sourcepath ${rootpath}/source)
set(installpath ${rootpath}/install)
set(buildpath ${rootpath}/build)

ocp_compress_extract(TARGET ${SOFTWARE} TAR ${SOFTWARE}-${VERSION} SOURCE
                     ${sourcepath})

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${installpath}
  COMMENT "Install the ${SOFTWARE}-${VERSION}")
# add_dependencies(install_${SOFTWARE} download_${SOFTWARE})

add_custom_command(
  OUTPUT ${installpath}
  WORKING_DIRECTORY ${sourcepath}
  DEPENDS ${sourcepath}
  COMMAND chmod 755 configure
  COMMAND
    ./configure --prefix=${installpath} --PETSC_DIR=${sourcepath} --with-cc=gcc
    --with-cxx=g++ --with-fc=gfortran --with-petsc4py=1 --download-mpi4py
    --download-fblaslapack --download-mpich --download-metis --download-parmetis
    --download-suitesparse --download-mumps --download-hypre --download-slepc
    --download-eigen --download-scalapack --download-superlu --download-fftw
  COMMAND make -j
  COMMAND make install
  VERBATIM USES_TERMINAL
  COMMENT "Build source and install.")

install(
  FILES ${SOFTWARE}-${VERSION}.tar.xz CMakeLists.txt
  DESTINATION toolkit/petsc/external
  COMPONENT ${PROJECT_NAME})

# PETSC_DIR=${OCP_CACHE}/petsc/3.18.4/install PETSC_ARCH=""
