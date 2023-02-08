# set(tkname gmsh)
# add_subdirectory(external)
# add_library(toolkit_${tkname} INTERFACE)
# add_library(OCP::toolkit_${tkname} ALIAS toolkit_${tkname})
# target_link_libraries(toolkit_${tkname} INTERFACE OCP::external_${tkname})

include(OCPToolkitInterface)
ocp_toolkit_interface()