set(SOFTWARE Qt)
set(VERSION 5.12.10)

set(rootpath ${OCP_CACHE}/${SOFTWARE}/${VERSION})
set(sourcepath ${rootpath}/source)
set(installpath ${rootpath}/install)
set(buildpath ${rootpath}/build)

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
    WORKING_DIRECTORY ${rootpath}
    COMMAND git clone https://code.qt.io/qt/qt5.git source
    COMMAND ${CMAKE_COMMAND} -E chdir source git checkout ${VERSION}
    COMMAND ${CMAKE_COMMAND} -E chdir source ./init-repository --module-subset=qtbase,qtx11extras
    VERBATIM USES_TERMINAL
    COMMENT "Download source") # --accept-messages

  add_custom_command(
    OUTPUT ${sourcepath}/qtbase/configure
    DEPENDS ${sourcepath}
    WORKING_DIRECTORY ${sourcepath}
    COMMAND git checkout ${VERSION}
    COMMAND ./init-repository --module-subset=qtbase,qtx11extras
    VERBATIM USES_TERMINAL
    COMMENT "Checkout to version ${VERSION}"
  )
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
      "${sourcepath}/configure" -prefix ${installpath} -release -opensource
      -confirm-license -nomake tools -nomake examples -nomake tests
    COMMAND make -j${ncore}
    COMMAND make install
    VERBATIM USES_TERMINAL
    COMMENT "Build source and install.")

  add_dependencies(install_Qt download_Qt)
  # else() add_custom_target( install_${SOFTWARE} DEPENDS
  # ${installpath}/${VERSION}/clang_64/lib COMMENT "Install the
  # ${SOFTWARE}-${VERSION}") endif()
endif()

install(
  FILES CMakeLists.txt
  DESTINATION toolkit/${SOFTWARE}/external
  COMPONENT ${PROJECT_NAME})