set(INSTALL_SCRIPT qt-unified-linux-x64-4.5.1-online.run)
set(SOFTWARE Qt)
set(VERSION 5.12.10)
string(REPLACE "." "" VERSION_SHORT ${VERSION})

add_custom_target(
  compress_Qt
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${INSTALL_SCRIPT}.tar.xz
  COMMENT "Compress the ${INSTALL_SCRIPT}")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${INSTALL_SCRIPT}.tar.xz
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND "${CMAKE_COMMAND}" "-E" "tar" "cJf" "${INSTALL_SCRIPT}.tar.xz" "--"
          "${INSTALL_SCRIPT}"
  COMMENT "XZ compress ${INSTALL_SCRIPT}")

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${INSTALL_SCRIPT})
  execute_process(
    COMMAND "${CMAKE_COMMAND}" "-E" "tar" "xJf" "${INSTALL_SCRIPT}.tar.xz"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/install/MaintenanceTool)
  set(install "${CMAKE_CURRENT_SOURCE_DIR}/install/MaintenanceTool")
else()
  set(install "./${INSTALL_SCRIPT}")
endif()

add_custom_target(
  download_${SOFTWARE}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/install/${VERSION}/Src
  COMMENT "Download the ${SOFTWARE}-${VERSION}")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/install/${VERSION}/Src
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND
    "${install}" "--accept-licenses" "--da" "--ao" "--confirm-command" "--root"
    "${OCP_ROOT}/toolkit/Qt/external/install" "--email" "0001001bill@gmail.com"
    "--password" "uyuy249lliB" "install" "qt.tools.ninja" "qt.tools.cmake"
    "qt.qt5.${VERSION_SHORT}.src"
  COMMENT "Download source")

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/install/${VERSION}/lib
  COMMENT "Install the ${SOFTWARE}-${VERSION}")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/${VERSION}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} -E make_directory build/${VERSION}
  COMMENT "Create the building directory")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/install/${VERSION}/lib
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${VERSION}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build/${VERSION}
  COMMAND
    "${CMAKE_CURRENT_SOURCE_DIR}/install/${VERSION}/Src/configure" -prefix
    ${OCP_ROOT}/toolkit/Qt/external/install/${VERSION} -release -opensource
    -confirm-license -nomake tools -nomake examples -nomake tests -skip qt3d
    -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity
    -skip qtdatavis3d -skip qtdeclarative -skip qtdoc -skip qtgamepad -skip
    qtgraphicaleffects -skip qtimageformats -skip qtlocation -skip qtmacextras
    -skip qtmultimedia -skip qtnetworkauth -skip qtpurchasing -skip
    qtquickcontrols -skip qtquickcontrols2 -skip qtscript -skip qtscxml -skip
    qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech
    -skip qttranslations -skip qtvirtualkeyboard -skip qtwayland -skip
    qtwebchannel -skip qtwebengine -skip qtwebsockets -skip qtwebview -skip
    qtwinextras -skip qtxmlpatterns
  COMMAND make -j
  COMMAND make install
  COMMENT "Build source and install. Be patient! This may take 30mins or more.")

install(
  FILES qt-unified-linux-x64-4.5.1-online.run.tar.xz CMakeLists.txt
  DESTINATION toolkit/Qt/external
  COMPONENT ${PROJECT_NAME})
