set(SOFTWARE VTK)
set(VERSION 8.1.0)
include(OCPCores)
include(OCPToolkitExternalCMake)

# if($(CMAKE_SYSTEM_NAME) MATCHES "Linux")
  set(qtpath
      ${OCP_CACHE}/Qt/5.12.10/install
      CACHE INTERNAL "" FORCE)
# else()
#   set(qtpath
#       ${OCP_CACHE}/Qt/5.12.10/install/5.12.10/clang_64
#       CACHE INTERNAL "" FORCE)
# endif()
if(EXISTS ${qtpath})
  ocp_toolkit_external_cmake(
    SOFTWARE
    ${SOFTWARE}
    VERSION
    ${VERSION}
    FLAGS
    "-DVTK_QT_VERSION:STRING=5"
    "-DQT_QMAKE_EXECUTABLE:PATH=${qtpath}/bin/qmake"
    "-DVTK_Group_Qt:BOOL=ON"
    "-DCMAKE_PREFIX_PATH:PATH=${qtpath}/lib/cmake"
    "-DBUILD_SHARED_LIBS:BOOL=ON")
else()
  ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION})
endif()

add_dependencies(install_VTK install_Qt)
