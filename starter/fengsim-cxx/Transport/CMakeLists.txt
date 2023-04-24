target_sources(
  FengSim PRIVATE data_analyze.cpp TransportDockWidget.cpp TransportDockWidget.ui
          )

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})
set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})