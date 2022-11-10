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


set(OCP_LIFECYCLE
    "CAE_Static_Console"
    CACHE STRING "The program life cycle that user want to use")
set_property(
  CACHE OCP_LIFECYCLE
  PROPERTY STRINGS
           CAE_Static_Console
           CAE_Static_GUI
           CAE_Dynamic_Console
           CAE_Dynamic_GUI
           CAM_Console
           CAM_GUI)