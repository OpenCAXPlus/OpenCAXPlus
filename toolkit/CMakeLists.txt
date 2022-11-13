# if(PROJECT_IS_TOP_LEVEL) cmake_minimum_required(VERSION 3.20)

# project(OpenCAXPlusToolkit) option(ENABLE_TEST "Enable testing" ON)
# if(ENABLE_TEST) include(CTest) endif() endif()

add_subdirectory(${OCP_ROOT}/framework/interface
                 ${PROJECT_BINARY_DIR}/framework/interface)

add_subdirectory(backend)
# add_subdirectory(frontend)

set(OCP_SDK_SRC
    ${OCP_SDK_SRC} ${OCP_TOOLKIT_SRC}
    CACHE INTERNAL "")
set(OCP_SDK_LINK
    ${OCP_SDK_LINK} ${OCP_TOOLKIT_LINK}
    CACHE INTERNAL "")
