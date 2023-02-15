set(SOFTWARE hdf5)
set(VERSION 20230108)
include(OCPToolkitExternalCMake)

ocp_toolkit_external_cmake(SOFTWARE ${SOFTWARE} VERSION ${VERSION} FLAGS
                           "-DHDF5_ENABLE_PARALLEL=ON")
