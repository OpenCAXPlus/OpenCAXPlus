macro(OCP_Embed_User_Program)
  set(options USE_CXX)
  set(oneValueArgs LIFECYCLE TARGET NAME ROOT)
  set(multiValueArgs COPYFILES TOOLS)
  cmake_parse_arguments(OCPUser "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(STATUS " Adding your program ...")
  message(STATUS " ROOT: ${OCPUser_ROOT}")
  message(STATUS " PROGRAM_NAME: ${OCPUser_NAME}")
  message(STATUS " TOOLS: ${OCPUser_TOOLS}")
  message(STATUS " LIFECYCLE: ${OCPUser_LIFECYCLE}")
  message(STATUS " COPYFILES: ${OCPUser_COPYFILES}")

  option(ENABLE_TEST "Enable testing" ON)
  if(ENABLE_TEST)
    include(CTest)
  endif()

  include(OCPPrepareVariables)

  set(OCP_LIFECYCLE
      ${OCPUser_LIFECYCLE}
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
  target_link_libraries(${OCPUser_TARGET} PUBLIC OCP::Toolkit)
  add_subdirectory(${OCP_ROOT}/framework/lifecycle
                   ${PROJECT_BINARY_DIR}/framework/lifecycle)

  add_custom_target(
    run
    DEPENDS ${USER_PROGRAM}
    COMMENT "Run the ${USER_PROGRAM}")

  add_custom_command(
    TARGET run
    COMMAND ${PROJECT_BINARY_DIR}/${USER_PROGRAM}/${USER_PROGRAM}
    VERBATIM)

  foreach(file IN LISTS ${OCPUser_COPYFILES})
    message(STATUS "Copy file "${file} " to "
                   ${PROJECT_BINARY_DIR}/${USER_PROGRAM}/${file})
    configure_file(${file} ${PROJECT_BINARY_DIR}/${USER_PROGRAM}/${file}
                   COPYONLY)
  endforeach()
endmacro(OCP_Embed_User_Program)
