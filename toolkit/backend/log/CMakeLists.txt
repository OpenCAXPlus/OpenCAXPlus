add_subdirectory(external)

# add_library(ocp_tk_log STATIC) add_library(OCP::Toolkit::Log ALIAS ocp_tk_log)

# target_sources(ocp_tk_log PUBLIC log.cpp log.h)
# target_link_libraries(ocp_tk_log PUBLIC spdlog)

set(OCP_TOOLKIT_SRC
    ${OCP_TOOLKIT_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    CACHE INTERNAL "")
set(OCP_TOOLKIT_LINK
    ${OCP_TOOLKIT_LINK} spdlog::spdlog
    CACHE INTERNAL "")
