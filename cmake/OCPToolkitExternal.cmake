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

  add_custom_target(
    compress_${OCPExternal_SOFTWARE}
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z
    COMMENT "Compress the ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")

  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND "${CMAKE_COMMAND}" "-E" "tar" "cf"
            "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z" "--format=7zip" "--" "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}/*"
    COMMENT "7zip compress ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}")


  if(NOT EXISTS
     ${CMAKE_CURRENT_SOURCE_DIR}/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION})
    execute_process(
      COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xf" 
              "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}.7z" "--format=7zip"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

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
