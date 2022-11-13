# add_library(ocp_fw_log STATIC) add_library(OCP::Framework::Log ALIAS
# ocp_fw_log)

# target_sources(ocp_fw_log PUBLIC log.c log.h)

set(OCP_FRAMEWORK_BACKEND_LOG_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    CACHE INTERNAL "")
