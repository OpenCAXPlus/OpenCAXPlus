if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(INSTALL_SCRIPT qt-unified-macOS-x64-4.5.2-online.dmg)
else()
  set(INSTALL_SCRIPT qt-unified-linux-x64-4.5.1-online.run)
endif()

set(SOFTWARE Qt)
set(VERSION 5.12.10)
string(REPLACE "." "" VERSION_SHORT ${VERSION})
include(OCPCompress)

set(rootpath ${OCP_CACHE}/${SOFTWARE}/${VERSION})
set(sourcepath ${rootpath}/source)
set(installpath ${rootpath}/install)
set(buildpath ${rootpath}/build)

ocp_compress_extract(TARGET ${SOFTWARE} TAR ${INSTALL_SCRIPT} SOURCE
                     ${rootpath}/${INSTALL_SCRIPT})

if(EXISTS ${installpath}/MaintenanceTool)
  set(install "${installpath}/MaintenanceTool")
else()
  set(install "${rootpath}/${INSTALL_SCRIPT}")
endif()

add_custom_target(
  install_${SOFTWARE}
  DEPENDS ${installpath}/lib
  COMMENT "Install the ${SOFTWARE}-${VERSION}")
  
# if($(CMAKE_SYSTEM_NAME) MATCHES "Linux")
if(NOT EXISTS ${sourcepath})
  add_custom_target(
    download_${SOFTWARE}
    DEPENDS ${sourcepath}
    COMMENT "Download the ${SOFTWARE}-${VERSION}")

  add_custom_command(
    OUTPUT ${sourcepath}
    DEPENDS ${install}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
      ${install} --verbose --accept-licenses --accept-obligations
      --confirm-command --default-answer --root ${sourcepath} --email
      $ENV{QT_EMAIL} --password $ENV{QT_PASSWORD} install qt.tools.ninja
      qt.tools.cmake qt.qt5.${VERSION_SHORT}.src
    VERBATIM USES_TERMINAL
    COMMENT "Download source") # --accept-messages


  # add_dependencies(install_${SOFTWARE} download_${SOFTWARE})

  add_custom_command(
    OUTPUT ${buildpath}
    WORKING_DIRECTORY ${rootpath}
    COMMAND ${CMAKE_COMMAND} -E make_directory build
    COMMENT "Create the building directory")

  add_custom_command(
    OUTPUT ${installpath}/lib
    WORKING_DIRECTORY ${buildpath}
    DEPENDS ${buildpath}
    COMMAND
      "${sourcepath}/${VERSION}/Src/configure" -prefix ${installpath} -release
      -opensource -confirm-license -nomake tools -nomake examples -nomake tests
      -skip qt3d -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip
      qtconnectivity -skip qtdatavis3d -skip qtdeclarative -skip qtdoc -skip
      qtgamepad -skip qtgraphicaleffects -skip qtimageformats -skip qtlocation
      -skip qtmacextras -skip qtmultimedia -skip qtnetworkauth -skip
      qtpurchasing -skip qtquickcontrols -skip qtquickcontrols2 -skip qtscript
      -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip
      qtspeech -skip qttranslations -skip qtvirtualkeyboard -skip qtwayland
      -skip qtwebchannel -skip qtwebengine -skip qtwebsockets -skip qtwebview
      -skip qtwinextras -skip qtxmlpatterns
    COMMAND make -j${ncore}
    COMMAND make install
    VERBATIM USES_TERMINAL
    COMMENT "Build source and install.")

  add_dependencies(install_Qt download_Qt)
# else()
#   add_custom_target(
#     install_${SOFTWARE}
#     DEPENDS ${installpath}/${VERSION}/clang_64/lib
#     COMMENT "Install the ${SOFTWARE}-${VERSION}")
# endif()
endif()

install(
  FILES qt-unified-linux-x64-4.5.1-online.run.tar.xz CMakeLists.txt
  DESTINATION toolkit/Qt/external
  COMPONENT ${PROJECT_NAME})

install(
  FILES qt-unified-macOS-x64-4.5.2-online.dmg.tar.xz CMakeLists.txt
  DESTINATION toolkit/Qt/external
  COMPONENT ${PROJECT_NAME})
