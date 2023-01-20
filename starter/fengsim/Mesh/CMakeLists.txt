target_sources(
  FengSim PRIVATE MeshGeneration.cpp MeshDockWidget.cpp MeshDockWidget.ui
          Slices.cpp)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})
set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})