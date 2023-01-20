target_sources(
  FengSim
  PRIVATE CADDockWidget.cpp
          CADDockWidget.ui
          OCCWidget.cpp
          PhysicsDockWidget.cpp
          PhysicsDockWidget.ui
          Primitive.cpp
          ViewWidget.cpp
          ViewWidget.ui)

target_include_directories(FengSim PUBLIC ${CMAK_CURRENT_SOURCE_DIR})

set_property(
  TARGET FengSim
  APPEND
  PROPERTY AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
