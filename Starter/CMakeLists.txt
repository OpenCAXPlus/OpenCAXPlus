# fasp solver
install(
  DIRECTORY FASPSolver
  COMPONENT ${PROJECT_NAME}
  DESTINATION ./starter
  PATTERN out EXCLUDE)
install(
  DIRECTORY .github
  DESTINATION ./starter/FASPSolver
  COMPONENT ${PROJECT_NAME})
install(
  FILES ../CMakePresets.json
  DESTINATION ./starter/FASPSolver
  COMPONENT ${PROJECT_NAME})

# CAE_Static_Console
install(
  DIRECTORY BasicConsole
  COMPONENT ${PROJECT_NAME}
  DESTINATION ./starter
  PATTERN out EXCLUDE)
install(
  DIRECTORY .github
  DESTINATION ./starter/BasicConsole
  COMPONENT ${PROJECT_NAME})
install(
  FILES ../CMakePresets.json
  DESTINATION ./starter/BasicConsole
  COMPONENT ${PROJECT_NAME})

add_subdirectory(M++)
