set(OCP_FRAMEWORK_SRC
    ""
    CACHE INTERNAL "" FORCE)
add_subdirectory(backend)
# add_subdirectory(frontend) add_subdirectory(communication)
# add_subdirectory(pbs)

set(OCP_SDK_SRC
    ${OCP_SDK_SRC} ${OCP_FRAMEWORK_SRC}
    CACHE INTERNAL "")

option(USER_CXX "The user program is using C++" OFF)
