# spdlog 1.11.0
add_subdirectory(external)

set(OCP_TOOLKIT_SRC
    ${OCP_TOOLKIT_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    CACHE INTERNAL "")

set(OCP_TOOLKIT_LINK
    ${OCP_TOOLKIT_LINK} spdlog::spdlog
    CACHE INTERNAL "")
