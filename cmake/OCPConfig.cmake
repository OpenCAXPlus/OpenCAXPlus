include(CMakeFindDependencyMacro)

set(OCP_ROOT "${OCP_DIR}/..")

list(APPEND CMAKE_MODULE_PATH "${OCP_ROOT}/cmake")

include(OCPEmbedUserProgram)

include(OCPPrepareVariables)
