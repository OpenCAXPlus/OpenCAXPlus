macro(ocp_compress_extract)

  set(options)
  set(oneValueArgs TARGET TAR SOURCE)
  set(multiValueArgs)

  cmake_parse_arguments(OCPCompress "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  add_custom_target(
    compress_${OCPCompress_TARGET}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${OCPCompress_TAR}.tar.xz
    COMMENT "Compress the ${OCPCompress_TAR}")

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${OCPCompress_TAR}.tar.xz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND "${CMAKE_COMMAND}" "-E" "echo" compress "${OCPCompress_SOURCE}" to
            "${CMAKE_CURRENT_SOURCE_DIR}/${OCPCompress_TAR}.tar.xz"
    COMMAND "${CMAKE_COMMAND}" "-E" "tar" "cJf" "${OCPCompress_TAR}.tar.xz" "--"
            ${OCPCompress_SOURCE}/*
    USES_TERMINAL
    COMMENT "XZ compress ${OCPCompress_TAR}")

  cmake_path(GET OCPCompress_SOURCE PARENT_PATH PARENT_DIR)
  cmake_path(GET OCPCompress_SOURCE FILENAME file)

  if(NOT EXISTS ${OCPCompress_SOURCE})
    add_custom_command(
      OUTPUT ${OCPCompress_SOURCE}
      WORKING_DIRECTORY ${PARENT_DIR}
      # COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" ${file}
      COMMAND
        "${CMAKE_COMMAND}" "-E" "echo" extract
        "${CMAKE_CURRENT_SOURCE_DIR}/${OCPCompress_TAR}.tar.xz" to
        "${PARENT_DIR}/${file}"
      COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xJf"
              "${CMAKE_CURRENT_SOURCE_DIR}/${OCPCompress_TAR}.tar.xz"
      COMMAND "${CMAKE_COMMAND}" "-E" "rename" "${OCPCompress_TAR}" "${file}"
      VERBATIM USES_TERMINAL
      COMMENT "XZ decompress ${OCPCompress_TAR}")
  endif()

endmacro()
