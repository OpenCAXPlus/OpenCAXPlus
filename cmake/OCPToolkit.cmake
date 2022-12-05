macro(OCP_Toolkit)
  set(options NO_EXTERNAL)
  set(oneValueArgs)
  set(multiValueArgs FILES)
  cmake_parse_arguments(OCPToolkit "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  file(RELATIVE_PATH OCPToolkit_NAME ${CMAKE_SOURCE_DIR}/toolkit
       ${CMAKE_CURRENT_SOURCE_DIR})

  add_library(toolkit_${OCPToolkit_NAME})
  add_library(OCP::Toolkit::${OCPToolkit_NAME} ALIAS toolkit_${OCPToolkit_NAME})

  target_sources(toolkit_${OCPToolkit_NAME} PUBLIC ${OCPToolkit_FILES})
  target_link_libraries(toolkit_${OCPToolkit_NAME} PUBLIC OCP::Interface)
  target_include_directories(toolkit_${OCPToolkit_NAME}
                             PUBLIC ${OCP_ROOT}/toolkit)

  if(NOT ${OCPToolkit_NO_EXTERNAL})
    target_link_libraries(toolkit_${OCPToolkit_NAME}
                          PUBLIC OCP::External::${OCPToolkit_NAME})
    message(STATUS "Using toolkit ${OCPToolkit_NAME}, " "has external, "
                   "source files ${OCPToolkit_FILES}")
    add_subdirectory(external)
  else()
    message(STATUS "Using toolkit ${OCPToolkit_NAME}, " "no external, "
                   "source files ${OCPToolkit_FILES}")
  endif()

  install(
    DIRECTORY .
    DESTINATION toolkit/${OCPToolkit_NAME}
    COMPONENT ${PROJECT_NAME}
    PATTERN "external" EXCLUDE)

endmacro(OCP_Toolkit)
