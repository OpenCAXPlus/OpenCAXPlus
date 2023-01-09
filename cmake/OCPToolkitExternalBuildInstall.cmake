macro(OCP_Toolkit_External_Build_Install)
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

  add_custom_target(
    install_${OCPExternal_SOFTWARE}
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}
    COMMENT "Compress the ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")

  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
      "${CMAKE_COMMAND}" "-S" "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}"
      "-B" "build/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" "-GNinja"
      "-DCMAKE_BUILD_TYPE=Release" "-DBUILD_TESTING=OFF"
      "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}"
      ${OCPExternal_FLAGS}
    COMMAND "${CMAKE_COMMAND}" "-E" "chdir"
            "build/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" "ninja"
    COMMAND
      "${CMAKE_COMMAND}" "-E" "chdir"
      "build/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" "ninja" "install"
    COMMENT "Build and install")

  install(
    DIRECTORY .
    DESTINATION Toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME}
    PATTERN "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" EXCLUDE)

endmacro(OCP_Toolkit_External)
