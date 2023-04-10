set(SOFTWARE petsc)
set(VERSION 3.18.4)
string(REPLACE "." "" VERSION_SHORT ${VERSION})
include(OCPCompress)

set(rootpath ${OCP_CACHE}/${SOFTWARE}/${VERSION})
set(sourcepath ${rootpath}/source)
set(installpath ${rootpath}/install)
set(buildpath ${rootpath}/build)

ocp_compress_extract(TARGET ${SOFTWARE} TAR ${SOFTWARE}-${VERSION} SOURCE
                     ${sourcepath} ANCHOR configure)

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${installpath}
  COMMENT "Install the ${SOFTWARE}-${VERSION}")
# add_dependencies(install_${SOFTWARE} download_${SOFTWARE})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(GCC gcc-12)
  set(G++ g++-12)
else()
  set(GCC gcc)
  set(G++ g++) 
endif()

add_custom_command(
  OUTPUT ${installpath}
  WORKING_DIRECTORY ${sourcepath}
  DEPENDS ${sourcepath}/configure
  COMMAND chmod 755 configure
  COMMAND
    ./configure --prefix=${installpath} --PETSC_DIR=${sourcepath} --with-cc=${GCC}
    --with-cxx=${G++} --with-fc=gfortran --with-petsc4py=1 --download-mpi4py
    --with-blas-lib=${OCP_CACHE}/lapack/3.11.0/install/lib/libblas.a
    --with-lapack-lib=${OCP_CACHE}/lapack/3.11.0/install/lib/liblapack.a
    --download-mpich --download-metis --download-parmetis --download-suitesparse
    --download-mumps --download-hypre --download-slepc --download-eigen
    --download-scalapack --download-superlu --download-fftw
  COMMAND make -j${ncore}
  COMMAND make install
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${rootpath}/build
  COMMAND ${CMAKE_COMMAND} -E rename ${sourcepath}/arch-linux-c-debug ${rootpath}/build
  VERBATIM USES_TERMINAL
  COMMENT "Build source and install.")

install(
  FILES ${SOFTWARE}-${VERSION}.tar.xz CMakeLists.txt
  DESTINATION toolkit/petsc/external
  COMPONENT ${PROJECT_NAME})

# PETSC_DIR=${OCP_CACHE}/petsc/3.18.4/install PETSC_ARCH=""

add_dependencies(install_petsc install_lapack)
