include(OCPToolkitExternalCustom)
set(SOFTWARE qtbase)
set(VERSION 5.15.2)
ocp_toolkit_external_custom(SOFTWARE ${SOFTWARE} VERSION ${VERSION})

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}
  COMMENT "Compress the ${SOFTWARE}-${VERSION}")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "build"
  COMMENT "Create build directory")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build
  COMMAND
    "../${SOFTWARE}-${VERSION}/configure" "-prefix"
    "${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}" "-opensource"
    "-confirm-license" "-release" "-nomake" "tests" "-nomake" "examples"
    "-nomake" "tools" "-make" "libs"
  COMMAND "make" "-j"
  COMMAND "make" "install"
  COMMENT "Custom build and install"
  VERBATIM)
