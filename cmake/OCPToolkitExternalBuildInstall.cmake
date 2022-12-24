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

  add_custom_target(
    compress_${OCPExternal_SOFTWARE}
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z
    COMMENT "Compress the ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")

  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
      "${CMAKE_COMMAND}" "-E" "tar" "cf"
      "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z" "--format=7zip" "--"
      "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}/*"
    COMMENT "7zip compress ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")

  if(NOT EXISTS
     ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION})
    execute_process(
      COMMAND
        "${CMAKE_COMMAND}" "-E" "tar" "xf"
        "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z" "--format=7zip"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

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
      "-B" "build" "-GNinja" "-DCMAKE_BUILD_TYPE=Release" "-DBUILD_TESTING=OFF"
      "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}"
      ${OCPExternal_FLAGS}
    COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build" "ninja"
    COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build" "ninja" "install"
    COMMENT "Build and install")

  install(
    DIRECTORY .
    DESTINATION Toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME}
    PATTERN "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" EXCLUDE)

endmacro(OCP_Toolkit_External)
