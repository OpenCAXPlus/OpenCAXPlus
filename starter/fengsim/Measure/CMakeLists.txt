target_sources(
  FengSim PRIVATE MeasureICP.cpp MeasureDockWidget.cpp MeasureDockWidget.ui
                  MeasureThread1.cpp MeasureThread3.cpp)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})
set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})