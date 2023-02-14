set(SOFTWARE VTK)
set(VERSION 8.1.0)
include(OCPToolkitExternalCMake)

set(qtpath
    ${OCP_CACHE}/Qt/5.12.10/install
    CACHE INTERNAL "" FORCE)

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
# add_library(external_VTK INTERFACE IMPORTED GLOBAL)
# add_library(OCP::external_VTK ALIAS external_VTK)

# if(EXISTS ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install)

# set_property( TARGET external_${SOFTWARE} APPEND PROPERTY
# INTERFACE_LINK_LIBRARIES fasp m)

# set_property( TARGET external_${SOFTWARE} APPEND PROPERTY
# INTERFACE_LINK_DIRECTORIES ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/lib)

# set_property( TARGET external_${SOFTWARE} APPEND PROPERTY
# INTERFACE_INCLUDE_DIRECTORIES
# ${OCP_CACHE}/${SOFTWARE}/${VERSION}/install/include) endif()
