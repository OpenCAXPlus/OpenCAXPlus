macro(OCP_Toolkit_interface)
  set(oneValueArgs)
  cmake_parse_arguments(OCPToolkit "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  if(${CMAKE_VERSION} VERSION_LESS 3.20)
    file(RELATIVE_PATH OCPToolkit_NAME ${CMAKE_SOURCE_DIR}/toolkit
         ${CMAKE_CURRENT_SOURCE_DIR})
  else()
    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM OCPToolkit_NAME)
  endif()

  set(tkname toolkit_${OCPToolkit_NAME})
  add_library(${tkname} INTERFACE)
  add_library(OCP::${tkname} ALIAS ${tkname})

  target_link_libraries(${tkname} INTERFACE OCP::external_${OCPToolkit_NAME})

  add_subdirectory(external)

  install(
    DIRECTORY .
    DESTINATION toolkit/${OCPToolkit_NAME}
    COMPONENT ${PROJECT_NAME}
    PATTERN "external" EXCLUDE)

  export(
    TARGETS ${tkname}
    NAMESPACE OCP::
    FILE ${tkname}Targets.cmake)

endmacro(OCP_Toolkit)
