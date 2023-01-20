target_sources(
  FengSim
  PRIVATE AdditiveManufacturingDockWidget.cpp
          AdditiveManufacturingDockWidget.ui AMConfig.cpp AMThread1.cpp
          AMThread2.cpp)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})

set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
