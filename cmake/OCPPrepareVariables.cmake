# set(OCP_SDK_SRC "" CACHE INTERNAL "" FORCE) set(OCP_SDK_LINK "" CACHE INTERNAL
# "" FORCE)

# set(OCP_FRAMEWORK_SRC "" CACHE INTERNAL "" FORCE)

set(OCP_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE FILEPATH "The OpenCAX+SDK root path" FORCE)

option(USER_CXX "The user program is using C++" OFF)

set(OCP_LIFECYCLE
    "basic"
    CACHE STRING "The program life cycle that user want to use" FORCE)

set_property(CACHE OCP_LIFECYCLE PROPERTY STRINGS BasicConsole GUI)

# set(OCP_TOOLKIT_SRC "" CACHE INTERNAL "" FORCE) set(OCP_TOOLKIT_LINK "" CACHE
# INTERNAL "" FORCE)

set(OCP_TOOLS
    ocplog faspsolver
    CACHE INTERNAL "" FORCE)

set(USER_PROGRAM
    "basic"
    CACHE STRING "The user program's name" FORCE)

set(USER_PROGRAM_TARGET
    ""
    CACHE STRING "The user program's target" FORCE)
