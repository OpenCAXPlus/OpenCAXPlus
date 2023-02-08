#
include(OCPToolkitExternalBuildInstall)

set(qtpath
    ${OCP_ROOT}/toolkit/Qt/external/install/5.12.10
    CACHE INTERNAL "" FORCE)

if(EXISTS ${qtpath})
  ocp_toolkit_external_build_install(
    SOFTWARE
    VTK
    VERSION
    8.1.0
    FLAGS
    "-DVTK_QT_VERSION:STRING=5"
    "-DQT_QMAKE_EXECUTABLE:PATH=${qtpath}/bin/qmake"
    "-DVTK_Group_Qt:BOOL=ON"
    "-DCMAKE_PREFIX_PATH:PATH=${qtpath}/lib/cmake"
    "-DBUILD_SHARED_LIBS:BOOL=ON")
else()
  ocp_toolkit_external_build_install(SOFTWARE VTK VERSION 8.1.0 FLAGS)
endif()
add_library(external_VTK INTERFACE IMPORTED GLOBAL)
add_library(OCP::external_VTK ALIAS external_VTK)
