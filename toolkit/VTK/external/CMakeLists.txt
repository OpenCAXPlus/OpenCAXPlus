#
include(OCPToolkitExternalBuildInstall)
ocp_toolkit_external_build_install(SOFTWARE VTK VERSION 8.2.0)

# set(VTK_DIR
#     ${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}/lib/cmake/vtk-8.2
# )

# find_package(VTK REQUIRED)
# vtk_module_config(VTK vtkCommonCore vtkFiltersSources vtkFiltersSources
#                   vtkInteractionStyle vtkRenderingOpenGL2)

# if(NOT VTK_FOUND)
#   message(FATAL_ERROR "CylinderExample: Unable to find the VTK build folder.")
# endif()

# # Prevent a "command line is too long" failure in Windows.
# set(CMAKE_NINJA_FORCE_RESPONSE_FILE
#     "ON"
#     CACHE BOOL "Force Ninja to use response files.")

    #external_VTK is created by the ocp_toolkit_external_build_install macro

add_library(external_VTK INTERFACE IMPORTED GLOBAL)
add_library(OCP::external_VTK ALIAS external_VTK)

# set_property(
#   TARGET external_VTK
#   APPEND
#   PROPERTY INTERFACE_LINK_LIBRARIES ${VTK_LIBRARIES})

# include(${VTK_USE_FILE})
