set(cliname ocp)
add_custom_target(
  cli ALL
  DEPENDS ${cliname}
  COMMENT "Build the cli target")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${cliname}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND go build -o ${cliname}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${OCP_BIN}
  COMMAND ${CMAKE_COMMAND} -E rename ${cliname} ${OCP_BIN}/${cliname}
  COMMENT "Build the OpenCAXPlusSDK cli"
  VERBATIM USES_TERMINAL)

install(
  FILES ${OCP_BIN}/${cliname}
  DESTINATION cli
  COMPONENT ${PROJECT_NAME})
