# message(STATUS The lifecycle folder "${PROJECT_BINARY_DIR}/${USER_PROGRAM}")
# add_subdirectory(${OCP_LIFECYCLE} ${PROJECT_BINARY_DIR}/${USER_PROGRAM})

add_subdirectory(basic_console)

set(VTK_DIR
    ${OCP_ROOT}/toolkit/VTK/external/install/VTK-8.2.0/lib/cmake/vtk-8.2)
find_package(VTK)

if(VTK_FOUND)
  message(STATUS "VTK found so build the basic_console_vtk lifecycle")
  add_subdirectory(basic_console_vtk)
endif()
