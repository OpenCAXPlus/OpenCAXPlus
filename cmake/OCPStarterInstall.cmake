macro(OCP_starter_install)
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs )
  cmake_parse_arguments(OCPStarter "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  install(
    DIRECTORY ${OCPStarter_NAME}
    COMPONENT ${PROJECT_NAME}
    DESTINATION starter
    PATTERN out EXCLUDE)
endmacro()
