# fasp solver
install(
  DIRECTORY FASPSolver
  DESTINATION ./starter
  PATTERN out EXCLUDE)
install(DIRECTORY .github DESTINATION ./starter/FASPSolver)
install(FILES ../CMakePresets.json DESTINATION ./starter/FASPSolver)

# CAE_Static_Console
install(
  DIRECTORY CAE_Static_Console
  DESTINATION ./starter
  PATTERN out EXCLUDE)
install(DIRECTORY .github DESTINATION ./starter/CAE_Static_Console)
install(FILES ../CMakePresets.json DESTINATION ./starter/CAE_Static_Console)
