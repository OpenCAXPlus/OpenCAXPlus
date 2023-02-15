macro(OCP_Toolkit_External_Cmake)
  set(options)
  set(oneValueArgs SOFTWARE VERSION SUBFOLDER)
  set(multiValueArgs FLAGS DEPENDS ENVS)

  cmake_parse_arguments(OCPExternal "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(
    STATUS "Add external library ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}"
  )

  set(rootpath ${OCP_CACHE}/${OCPExternal_SOFTWARE}/${OCPExternal_VERSION})
  set(installpath ${rootpath}/install)
  set(buildpath ${rootpath}/build)
  set(sourcepath ${rootpath}/source)
  set(tarname ${OCPExternal_SOFTWARE}-${OCPExternal_VERSION})
  include(OCPCompress)

  ocp_compress_extract(TARGET ${OCPExternal_SOFTWARE} TAR ${tarname} SOURCE
                       ${sourcepath})

  set(ENV{PETSC_DIR} ${OCP_CACHE}/petsc/3.18.4/install)
  set(ENV{PETSC_ARCH} "")

  add_custom_target(
    install_${OCPExternal_SOFTWARE}
    DEPENDS ${installpath} ${OCPExternal_DEPENDS}
    COMMENT "Compress the ${tarname}")

  add_custom_command(
    OUTPUT ${installpath}
    DEPENDS ${sourcepath}
    WORKING_DIRECTORY ${sourcepath}
    COMMAND
      ${CMAKE_COMMAND} -E env ${OCPExternal_ENVS} "${CMAKE_COMMAND}" "-S"
      "${sourcepath}/${OCPExternal_SUBFOLDER}" "-B" "${buildpath}" "-GNinja"
      "-DCMAKE_BUILD_TYPE=Release" "-DBUILD_TESTING=OFF"
      "-DCMAKE_INSTALL_PREFIX=${installpath}" ${OCPExternal_FLAGS}
    COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${buildpath}" "ninja"
    COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${buildpath}" "ninja" "install"
    VERBATIM USES_TERMINAL
    COMMENT "Build and install")

  add_library(external_${OCPExternal_SOFTWARE} INTERFACE IMPORTED GLOBAL)
  add_library(OCP::external_${OCPExternal_SOFTWARE} ALIAS
              external_${OCPExternal_SOFTWARE})

  # if(EXISTS ${installpath}) set_property( TARGET
  # external_${OCPExternal_SOFTWARE} APPEND PROPERTY INTERFACE_LINK_LIBRARIES
  # ${OCPExternal_LIBRARIES})

  # set_property( TARGET external_${OCPExternal_SOFTWARE} APPEND PROPERTY
  # INTERFACE_INCLUDE_DIRECTORIES ${installpath}/include
  # ${OCPExternal_INCLUDE_DIRECTORIES}) endif()

  install(
    FILES ${tarname}.tar.xz CMakeLists.txt
    DESTINATION toolkit/${OCPExternal_SOFTWARE}/external
    COMPONENT ${PROJECT_NAME})

endmacro(OCP_Toolkit_External_Cmake)
