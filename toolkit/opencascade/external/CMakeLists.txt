#
include(OCPToolkitExternalDownload)

ocp_toolkit_external_download(
  URL
  https://ocp-toolkit.oss-cn-hongkong.aliyuncs.com/opencascade-7.7.0.tar.xz
  SOFTWARE
  opencascade
  VERSION
  7.7.0
  FLAGS
  "-DOCE_INSTALL_PREFIX=${OCP_CACHE}/opencascade/7.7.0/install"
  "-DOCE_WITH_VTK=ON"
  "-DVTK_DIR=${OCP_CACHE}/VTK/8.1.0/install/lib/cmake/vtk-8.1")

# add_library(external_OCE INTERFACE IMPORTED GLOBAL)
# add_library(OCP::external_OCE ALIAS external_OCE)

add_dependencies(install_OCE install_Qt install_VTK)
