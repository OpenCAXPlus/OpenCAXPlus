set(SOFTWARE gmsh)
set(VERSION 3.0.6)
include(OCPToolkitExternalCustom)

ocp_toolkit_external_custom(SOFTWARE ${SOFTWARE} VERSION ${VERSION})

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}
  COMMENT "Compress the ${SOFTWARE}-${VERSION}")

# add_custom_command( OUTPUT
# ${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}
# WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND "${CMAKE_COMMAND}" "-S"
# "${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}" "-B" "build"
# "-GNinja""-DCMAKE_BUILD_TYPE=Release" "-DENABLE_BUILD_DYNAMIC=1"
# "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install/${OCPExternal_SOFTWARE}-${OCPExternal_VERSION}"
# ${OCPExternal_FLAGS} COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build" "ninja"
# COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build" "ninja" "install" COMMENT
# "Build and install")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND
    "${CMAKE_COMMAND}" "-S" "${SOFTWARE}-${VERSION}" "-B"
    "build/${SOFTWARE}-${VERSION}" "-DCMAKE_BUILD_TYPE=Release" "-DDEFAULT=0"
    "-DENABLE_BUILD_LIB=0" "-DENABLE_POST=1" "-DENABLE_PARSER=1"
    "-DENABLE_BUILD_SHARED=1" "-DENABLE_BUILD_DYNAMIC=1"
    "-DCMAKE_LIBRARY_PATH=${OCP_ROOT}/toolkit/lapack/external/install/lapack-3.11.0"
    "-DCMAKE_C_FLAGS=-lgfortran"
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install/${SOFTWARE}-${VERSION}"
  COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build/${SOFTWARE}-${VERSION}" "make"
  COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "build/${SOFTWARE}-${VERSION}" "make"
          "install"
  COMMENT "Build and install")

  