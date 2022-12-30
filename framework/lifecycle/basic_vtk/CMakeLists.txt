if(OCP_SDK_INSIDE)
  set(USER_PROGRAM basic_vtk)
endif()

add_executable(${USER_PROGRAM})
target_link_libraries(${USER_PROGRAM} PUBLIC OCP::Interface)
target_include_directories(${USER_PROGRAM} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${USER_PROGRAM} PUBLIC main.c)

set(VTK_DIR
    ${OCP_ROOT}/toolkit/VTK/external/install/VTK-8.2.0/lib/cmake/vtk-8.2)
find_package(VTK REQUIRED)
vtk_module_config(VTK vtkCommonCore vtkFiltersSources vtkFiltersSources
                  vtkInteractionStyle vtkRenderingOpenGL2)
include(${VTK_USE_FILE})

if(OCP_SDK_INSIDE)
  target_sources(${USER_PROGRAM} PUBLIC dummy.c)
  foreach(target ${OCP_TOOLS})
    set_property(
      TARGET ${USER_PROGRAM}
      APPEND
      PROPERTY LINK_LIBRARIES OCP::toolkit_${target})
  endforeach()
  target_link_libraries(${USER_PROGRAM} PUBLIC ${VTK_LIBRARIES})
else()
  target_link_libraries(${USER_PROGRAM} PUBLIC ${USER_PROGRAM_TARGET})
  target_link_libraries(${USER_PROGRAM_TARGET} PUBLIC ${VTK_LIBRARIES})
endif()

if(ENABLE_TEST)
  add_test(
    NAME ${USER_PROGRAM}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${USER_PROGRAM})
endif()
