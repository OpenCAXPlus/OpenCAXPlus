add_library(ocp_interface INTERFACE)
add_library(OCP::Interface ALIAS ocp_interface)
target_include_directories(ocp_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
