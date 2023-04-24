cmake_minimum_required(VERSION 3.16)

project(mppstarter)

# set(CMAKE_CXX_COMPILER /usr/bin/mpicxx.openmpi)

set(OCP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
# message(${OpenCAXPlusSDK_DIR})
find_package(OCP)
find_package(MPI REQUIRED)

add_library(mppstarter STATIC)
target_sources(mppstarter PUBLIC App.cpp Poisson.cpp PoissonProblems.cpp Define.cpp)

								 
# target_compile_options(-diag-disable=10441)
ocp_embed_user_program(
 LIFECYCLE
 "basic"
 TOOLS
 "spdlog;M++;faspsolver"
 NAME
 "mpp"
 TARGET
 mppstarter
 ROOT
 "${OCP_ROOT}"
 COPYFILES
 poisson.conf
 example2.geo
 USE_CXX)
# set(MPPLib_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../build/toolkit/M++/libM++.a")


# include_directories(${OpenCAXPlusSDK_DIR})

# include_directories("src")

# aux_source_directory(src M++_DIR_SRCS)
# message(${M++_DIR_SRCS})

# add_executable (PoissonRun ${M++_DIR_SRCS})

# message(${MPPLib_DIR})
# target_link_libraries (PoissonRun ${MPPLib_DIR} /usr/local/lib/libblas.so /usr/local/lib/liblapack.so ${MPI_LIBRARIES} /usr/local/lib/libsuperlu.a)

#file(COPY "log" DESTINATION "./../../")
#file(COPY "data/vtk" DESTINATION "./../../data")
#file(COPY "conf" DESTINATION "./../../")
#file(COPY "solver/conf" DESTINATION "./../../solver/")
