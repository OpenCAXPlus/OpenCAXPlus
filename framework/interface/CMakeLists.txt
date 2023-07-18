include(GNUInstallDirs)

set(OCP_INTERFACE Interface)
add_library(${OCP_INTERFACE} INTERFACE)
add_library(OCP::Interface ALIAS ${OCP_INTERFACE})
target_include_directories(${OCP_INTERFACE} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
    TARGETS ${OCP_INTERFACE}
    EXPORT ${OCP_INTERFACE}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT ${OCP_INTERFACE}Targets
    FILE ${OCP_INTERFACE}Targets.cmake
    NAMESPACE OCP::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ocp)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${OCP_INTERFACE}Config.cmake.in
        "${OCP_INTERFACE}Config.cmake"
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/ocp)

write_basic_package_version_file(
    ${OCP_INTERFACE}ConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${OCP_INTERFACE}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${OCP_INTERFACE}ConfigVersion.cmake"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/ocp)

install(
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/ocp"
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}")

export(EXPORT ${OCP_INTERFACE}Targets NAMESPACE OCP:: FILE ${OCP_INTERFACE}Targets.cmake)