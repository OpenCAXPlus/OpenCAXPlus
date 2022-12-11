macro(OCP_Embed_User_Program)
  set(options USE_CXX)
  set(oneValueArgs LIFECYCLE TARGET NAME ROOT)
  set(multiValueArgs COPYFILES TOOLS)
  cmake_parse_arguments(OCPUser "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  message(
    STATUS "Adding your app ${OCPUser_NAME}, "
           "using lifecycle ${OCPUser_LIFECYCLE}, "
           "calling target ${OCPUser_TARGET}, "
           "copying files ${OCPUser_COPYFILES}, "
           "with tools ${OCPUser_TOOLS}, "
           "OCP root at ${OCPUser_ROOT}")

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

  target_include_directories(
    ${OCPUser_TARGET} PUBLIC ${OCP_ROOT}/toolkit
                             ${OCP_ROOT}/framework/lifecycle/${OCPUser_LIFECYCLE})

  # add_subdirectory(${OCP_ROOT}/toolkit ${PROJECT_BINARY_DIR}/toolkit)

  add_subdirectory(${OCP_ROOT}/framework/interface ${PROJECT_BINARY_DIR}/interface)

  target_link_libraries(${OCPUser_TARGET} PUBLIC OCP::Interface)

  foreach(target ${OCP_TOOLS})
    message(STATUS "Adding tool " ${target})
    if(NOT TARGET toolkit_${target})
      add_subdirectory(${OCP_ROOT}/toolkit/${target}
                       ${PROJECT_BINARY_DIR}/toolkit/${target})
    endif()
    target_link_libraries(${OCPUser_TARGET} PUBLIC OCP::toolkit_${target})
  endforeach()

  add_subdirectory(${OCP_ROOT}/framework/lifecycle/${OCP_LIFECYCLE}
                   ${PROJECT_BINARY_DIR}/${USER_PROGRAM})

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
