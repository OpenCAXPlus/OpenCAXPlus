add_custom_target(
    compress_${OCPExternal_SOFTWARE}
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.xz
    COMMENT "Compress the ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")

  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.xz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
      "${CMAKE_COMMAND}" "-E" "tar" "cJf"
      "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.xz"  "--"
      "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}/*"
    COMMENT "XZ compress ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" VERBATIM)

  if(NOT EXISTS
     ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION})
    execute_process(
      COMMAND
        "${CMAKE_COMMAND}" "-E" "tar" "xJf"
        "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.tar.xz"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
