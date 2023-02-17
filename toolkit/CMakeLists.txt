add_subdirectory(ocplog)
add_subdirectory(ocpparam)
add_subdirectory(faspsolver)
add_subdirectory(M++)
add_subdirectory(spdlog)

add_subdirectory(lapack)
add_subdirectory(gmsh)

add_subdirectory(Qt)
add_subdirectory(VTK)
add_subdirectory(OCE)

add_subdirectory(pugixml)
add_subdirectory(petsc)
add_subdirectory(hdf5)
# The following commented could be configured using petsc
# add_subdirectory(parmetis)
add_subdirectory(basix)
add_subdirectory(ffcx)
add_subdirectory(dolfinx)

add_custom_target(install_all COMMENT "Install all tools")
add_dependencies(
  install_all
  install_faspsolver
  install_M++
  install_spdlog
  install_lapack
  install_gmsh
  install_Qt
  install_VTK
  install_OCE
  install_pugixml
  install_petsc
  install_hdf5
  install_basix
  install_ffcx
  install_dolfinx)
