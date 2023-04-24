target_sources(FengSim PRIVATE OCPoroDialog.cpp OCPoroDockWidget.cpp
                               OCPoroDockWidget.ui OCPoroDialog.ui)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})
set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
