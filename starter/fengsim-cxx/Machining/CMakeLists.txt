target_sources(
  FengSim PRIVATE MakeTools.cpp MachiningDockWidget.cpp MachiningDockWidget.ui
                  machining.qrc MachiningThread1.cpp MachiningThread2.cpp)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})
set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})