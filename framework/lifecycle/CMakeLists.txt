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

add_subdirectory(${OCP_LIFECYCLE})
