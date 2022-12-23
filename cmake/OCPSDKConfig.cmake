include(CMakeFindDependencyMacro)

set(OCP_ROOT "${OCPSDK_DIR}/..")

list(APPEND CMAKE_MODULE_PATH "${OCP_ROOT}/cmake")

include(OCPEmbedUserProgram)