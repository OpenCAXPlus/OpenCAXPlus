macro(OCP_Toolkit)
  set(options NO_EXTERNAL)
  set(oneValueArgs)
  set(multiValueArgs FILES TOOLS)
  cmake_parse_arguments(OCPToolkit "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  if (${CMAKE_VERSION} VERSION_LESS 3.20)
	file(RELATIVE_PATH OCPToolkit_NAME ${CMAKE_SOURCE_DIR}/Toolkit ${CMAKE_CURRENT_SOURCE_DIR})
  else ()
	cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM OCPToolkit_NAME)
  endif()

  set(tkname toolkit_${OCPToolkit_NAME})
  add_library(${tkname})
  add_library(OCP::${tkname} ALIAS ${tkname})

  target_sources(${tkname} PUBLIC ${OCPToolkit_FILES})
  target_link_libraries(${tkname} PUBLIC OCP::Interface)
  target_include_directories(${tkname} PUBLIC ${OCP_ROOT}/Toolkit)

  foreach(target ${OCPToolkit_TOOLS})
    # set(toolkit_${target}_DIR ${PROJECT_BINARY_DIR}/toolkit/${target})
    message(STATUS "Using tool " ${target} " " ${toolkit_${target}_DIR})
    if(NOT TARGET toolkit_${target})
      add_subdirectory(${OCP_ROOT}/Toolkit/${target}
                       ${PROJECT_BINARY_DIR}/Toolkit/${target})
    endif()
    target_link_libraries(${tkname} PUBLIC OCP::toolkit_${target})
  endforeach()

  if(NOT ${OCPToolkit_NO_EXTERNAL})
    target_link_libraries(${tkname} PUBLIC OCP::external_${OCPToolkit_NAME})
    message(STATUS "Using toolkit ${OCPToolkit_NAME}, " "has external, "
                   "source files ${OCPToolkit_FILES}"
                   "link with tools ${OCPToolkit_TOOLS}")

    add_subdirectory(external)
  else()
    message(STATUS "Using toolkit ${OCPToolkit_NAME}, " "no external, "
                   "source files ${OCPToolkit_FILES}, "
                   "link with tools ${OCPToolkit_TOOLS}")
  endif()

  install(
    DIRECTORY .
    DESTINATION Toolkit/${OCPToolkit_NAME}
    COMPONENT ${PROJECT_NAME}
    PATTERN "external" EXCLUDE)

  export(
    TARGETS ${tkname}
    NAMESPACE OCP::
    FILE ${tkname}Targets.cmake)

endmacro(OCP_Toolkit)
