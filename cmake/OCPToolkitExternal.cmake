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

  include(OCPCompress)


  add_subdirectory(${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}) # version
                                                                   # 1.11.0

  add_library(external_${OCPExternal_SOFTWARE} INTERFACE IMPORTED GLOBAL)
  add_library(OCP::external_${OCPExternal_SOFTWARE} ALIAS external_${OCPExternal_SOFTWARE})

  set_property(
    TARGET external_${OCPExternal_SOFTWARE}
    APPEND
    PROPERTY INTERFACE_LINK_LIBRARIES ${OCPExternal_LIBRARIES})

  set_property(
    TARGET external_${OCPExternal_SOFTWARE}
    APPEND
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OCPExternal_INCLUDE_DIRECTORIES})

  install(
    DIRECTORY .
    DESTINATION Toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME}
    PATTERN "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" EXCLUDE)

endmacro(OCP_Toolkit_External)
