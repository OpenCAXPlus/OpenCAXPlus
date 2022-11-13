set(OCP_SDK_SRC
    ""
    CACHE INTERNAL "" FORCE)
set(OCP_SDK_LINK
    ""
    CACHE INTERNAL "" FORCE)

set(OCP_FRAMEWORK_SRC
    ""
    CACHE INTERNAL "" FORCE)

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE FILEPATH "The OpenCAX+SDK root path" FORCE)

option(USER_CXX "The user program is using C++" OFF)

set(OCP_LIFECYCLE
    "CAE_Static_Console"
    CACHE STRING "The program life cycle that user want to use" FORCE)
set_property(
  CACHE OCP_LIFECYCLE
  PROPERTY STRINGS
           CAE_Static_Console
           CAE_Static_GUI
           CAE_Dynamic_Console
           CAE_Dynamic_GUI
           CAM_Console
           CAM_GUI)

set(OCP_BACKEND_LOG
    "default"
    CACHE STRING "The choice of OCP SDK backend log" FORCE)
set_property(CACHE OCP_BACKEND_LOG PROPERTY STRINGS default spdlog)

set(OCP_BACKEND_SOLVER
    "faspsolver"
    CACHE STRING "The choice of OCP SDK backend solver" FORCE)
set_property(CACHE OCP_BACKEND_SOLVER PROPERTY STRINGS faspsolver)

set(OCP_TOOLKIT_SRC
    ""
    CACHE INTERNAL "" FORCE)
set(OCP_TOOLKIT_LINK
    ""
    CACHE INTERNAL "" FORCE)

set(USER_PROGRAM
    "CAEStaticConsole"
    CACHE STRING "The user program's name" FORCE)
