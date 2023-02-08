macro(OCP_Toolkit_External_Custom)
  set(options)
  set(oneValueArgs SOFTWARE VERSION)
  set(multiValueArgs FLAGS)

  cmake_parse_arguments(OCPExternal "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(
    STATUS
      "Add external library ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}, link to ${OCPExternal_LIBRARIES}, includes ${OCPExternal_INCLUDE_DIRECTORIES}"
  )

  include(OCPCompress)

  # install( DIRECTORY . DESTINATION Toolkit/${OCPExternal_SOFTWARE}/external
  # COMPONENT ${PROJECT_NAME} PATTERN
  # "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" EXCLUDE)

  install(
    FILES ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.xz CMakeLists.txt
    DESTINATION toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME})

endmacro(OCP_Toolkit_External)
