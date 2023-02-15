set(SOFTWARE parmetis)
set(VERSION 4.0.3)
include(OCPToolkitExternalCMake)

# find_package(MPI REQUIRED)
# include_directories(${MPI_C_INCLUDE_DIRS})
# message(STATUS MPI ${MPI_C_INCLUDE_DIRS})
ocp_toolkit_external_cmake(
  SOFTWARE
  ${SOFTWARE}
  VERSION
  ${VERSION}
  FLAGS
  "-DMETIS_PATH=metis"
  "-DGKLIB_PATH=metis/GKlib"
  "-DCMAKE_C_COMPILER=mpicc")
