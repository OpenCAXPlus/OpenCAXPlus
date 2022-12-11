add_custom_target(
  html 
  DEPENDS _build/html/index.html
  COMMENT "Build the cli target")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/_build/html/index.html
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND make html
  COMMENT "Build the OpenCAXPlusSDK webpage documentation"
  VERBATIM)

add_custom_target(
  latexpdf 
  DEPENDS _build/latex/opencaxplussdk.pdf
  COMMENT "Build the cli target")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/_build/latex/opencaxplussdk.pdf
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND make latexpdf
  COMMENT "Build the OpenCAXPlusSDK pdf documentation"
  VERBATIM)
