cmake_minimum_required(VERSION 3.21)

project(OpenCAXPlusToolkit)

if(PROJECT_IS_TOP_LEVEL)
  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()
endif()

set(OCP_TOOLKIT_SRC
    ""
    CACHE INTERNAL "" FORCE)
set(OCP_TOOLKIT_LINK
    ""
    CACHE INTERNAL "" FORCE)
add_subdirectory(backend)
# add_subdirectory(frontend)

set(OCP_SDK_SRC
    ${OCP_SDK_SRC} ${OCP_TOOLKIT_SRC}
    CACHE INTERNAL "")
set(OCP_SDK_LINK
    ${OCP_SDK_LINK} ${OCP_TOOLKIT_LINK}
    CACHE INTERNAL "")
