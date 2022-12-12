cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(m++_api)

# set(CMAKE_CXX_COMPILER /usr/bin/mpicxx.openmpi)

add_library(M++ STATIC)

find_package(MPI)
if(MPI_FOUND)
  message(STATUS "MPI FOUND")
  message(STATUS ${MPI_CXX_INCLUDE_DIRS})
  target_include_directories(M++ PUBLIC ${MPI_CXX_INCLUDE_DIRS})
  target_compile_definitions(M++ PUBLIC ${MPI_CXX_COMPILE_DEFINITIONS})
  target_link_libraries(M++ PUBLIC ${MPI_CXX_LIBRARIES})
endif()

target_sources(
  M++
  PUBLIC src/Identify.C
         src/MatrixGraph.C
         src/Algebra.C
         src/Shape.C
         src/Point.C
         src/Parallel.C
         src/Time.C
         src/ctools.C
         src/IO.C
         src/Mesh.C
         src/Cell.C
         src/Distribution.C
         src/Plot.C
         src/Quadrature.C
         src/DoF.C
         src/Sparse.C
         src/Interface.C
         src/Discretization.C
         src/LinearSolver.C
         src/DD.C
         src/Preconditioner.C
         src/TimeSeries.C
         src/ESolver.C
         src/Transfer.C
         src/Element.C
         src/FaceElement.C
         src/Newton.C
         src/Newmark.C
         src/Euler.C
         src/Small.C
         src/Schur.C)

target_include_directories(M++ PUBLIC "src")

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

target_link_libraries(M++ PUBLIC ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES}
                                 ${MPI_LIBRARIES} superlu)

# target_include_directories(M++ PUBLIC "src")

# target_link_libraries( M++ PUBLIC /usr/local/lib/libblas.so
# /usr/local/lib/liblapack.so ${MPI_LIBRARIES} /usr/local/lib/libsuperlu.a)

target_compile_options(M++ PRIVATE -Wno-deprecated)
# file(COPY "log" DESTINATION "./../../") file(COPY "data/vtk" DESTINATION
# "./../../data") file(COPY "conf" DESTINATION "./../../") file(COPY
# "solver/conf" DESTINATION
# "./../../solver/")
