cmake_minimum_required(VERSION 3.20)

project(mppstarter)

set(OpenCAXPlusSDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
find_package(OpenCAXPlusSDK)

add_library(mppstarter STATIC)
target_sources(mppstarter PUBLIC src/Main.C src/Poisson.C src/PoissonProblems.C)

ocp_embed_user_program(
  LIFECYCLE
  "CAE_Static_Console"
  TOOLS
  "default_log;M++"
  NAME
  "Mpp"
  TARGET
  mppstarter
  ROOT
  "${OCP_ROOT}"
  COPYFILES
  conf/poisson.conf)
