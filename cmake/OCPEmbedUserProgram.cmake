macro(OCP_Embed_User_Program)
  set(options USE_CXX)
  set(oneValueArgs LIFECYCLE LOG SOLVER TARGET NAME ROOT)
  cmake_parse_arguments(OCPUser "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(STATUS " Adding your program ...")
  message(STATUS " ROOT: ${OCPUser_ROOT}")
  message(STATUS " PROGRAM_NAME: ${OCPUser_NAME}")
  message(STATUS " LOG: ${OCPUser_LOG}")
  message(STATUS " SOLVER: ${OCPUser_SOLVER}")
  message(STATUS " LIFECYCLE: ${OCPUser_LIFECYCLE}")

  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()

  include(OCPPrepareVariables)

  set(OCP_LIFECYCLE
      ${OCPUser_LIFECYCLE}
      CACHE INTERNAL "")

  set(OCP_BACKEND_LOG
      ${OCPUser_LOG}
      CACHE INTERNAL "")

  set(OCP_BACKEND_SOLVER
      ${OCPUser_SOLVER}
      CACHE INTERNAL "")

  set(USER_PROGRAM
      ${OCPUser_NAME}
      CACHE INTERNAL "")

  set(OCP_ROOT
      ${OCPUser_ROOT}
      CACHE INTERNAL "")

  set(USER_PROGRAM_TARGET
      ${OCPUser_TARGET}
      CACHE INTERNAL "")

  set(USER_CXX
      ${OCPUser_USE_CXX}
      CACHE BOOL "" FORCE)

  add_subdirectory(${OCP_ROOT}/toolkit ${PROJECT_BINARY_DIR}/toolkit)

  target_include_directories(
    ${OCPUser_TARGET} PUBLIC ${OCP_ROOT}/toolkit
                             ${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE})
  target_link_libraries(${OCPUser_TARGET} PUBLIC OCP::Toolkit::Log
                                                 OCP::Toolkit::Solver)
  add_subdirectory(${OCP_ROOT}/framework/lifecycle
                   ${PROJECT_BINARY_DIR}/framework/lifecycle)
endmacro(OCP_Embed_User_Program)
