macro(OCP_Custom_User_Program)
  set(options USE_CXX)
  set(oneValueArgs LOG SOLVER NAME ROOT)
  cmake_parse_arguments(OCPUser "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(STATUS " Adding your program ...")
  message(STATUS " ROOT: ${OCPUser_ROOT}")
  message(STATUS " PROGRAM_NAME: ${OCPUser_NAME}")
  message(STATUS " TOOLS: ${OCPUser_TOOLS}")

  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()

  include(OCPPrepareVariables)

  set(OCP_LIFECYCLE
      Custom_Console
      CACHE INTERNAL "")

  set(OCP_TOOLS
      ${OCPUser_TOOLS}
      CACHE INTERNAL "")

  set(USER_PROGRAM
      ${OCPUser_NAME}
      CACHE INTERNAL "")

  set(OCP_ROOT
      ${OCPUser_ROOT}
      CACHE INTERNAL "")

  add_subdirectory(${OCP_ROOT}/toolkit ${PROJECT_BINARY_DIR}/toolkit)

  target_include_directories(
    ${OCPUser_TARGET} PUBLIC ${OCP_ROOT}/toolkit
                             ${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE})
  target_link_libraries(${OCPUser_TARGET} PUBLIC OCP::Toolkit)
  add_subdirectory(${OCP_ROOT}/framework/lifecycle
                   ${PROJECT_BINARY_DIR}/framework/lifecycle)

endmacro(OCP_Custom_User_Program)
