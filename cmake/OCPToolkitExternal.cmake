macro(OCP_Toolkit_External)
  set(options)
  set(oneValueArgs SOFTWARE VERSION)
  cmake_parse_arguments(OCPUser "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(STATUS " Adding external library ...")
  message(STATUS " Software : ${OCPUser_SOFTWARE}")
  message(STATUS " Version  : ${OCPUser_VERSION}")

  if(NOT EXISTS
     ${CMAKE_CURRENT_SOURCE_DIR}/${OCPUser_SOFTWARE}-${OCPUser_VERSION})
    execute_process(
      COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xz"
              "${OCPUser_SOFTWARE}-${OCPUser_VERSION}.tar.gz"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog) message(STATUS "spdlog exist")
  # endif()

  add_subdirectory(${OCPUser_SOFTWARE}-${OCPUser_VERSION}) # version 1.11.0

  install(
    DIRECTORY .
    DESTINATION toolkit/${OCPUser_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME}
    PATTERN "${OCPUser_SOFTWARE}-${OCPUser_VERSION}" EXCLUDE)

endmacro(OCP_Toolkit_External)
