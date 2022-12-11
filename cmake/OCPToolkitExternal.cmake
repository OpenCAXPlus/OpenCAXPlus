macro(OCP_Toolkit_External)
  set(options)
  set(oneValueArgs SOFTWARE VERSION)
  set(multiValueArgs LIBRARIES INCLUDE_DIRECTORIES)

  cmake_parse_arguments(OCPExternal "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(
    STATUS
      "Add external library ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}, link to ${OCPExternal_LIBRARIES}, includes ${OCPExternal_INCLUDE_DIRECTORIES}"
  )

  if(NOT EXISTS
     ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION})
    execute_process(
      COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xz"
              "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.gz"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  add_subdirectory(${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}) # version
                                                                   # 1.11.0

  add_library(external_${external_name} INTERFACE IMPORTED GLOBAL)
  add_library(OCP::External::${external_name} ALIAS external_${external_name})

  set_property(
    TARGET external_${external_name}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES ${OCPExternal_LIBRARIES})

  set_property(
    TARGET external_${external_name}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OCPExternal_INCLUDE_DIRECTORIES})

  install(
    DIRECTORY .
    DESTINATION Toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME}
    PATTERN "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" EXCLUDE)

endmacro(OCP_Toolkit_External)
