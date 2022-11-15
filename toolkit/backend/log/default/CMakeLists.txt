# set(OCP_TOOLKIT_SRC ${OCP_TOOLKIT_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/log.h CACHE
# INTERNAL "")

# set(OCP_TOOLKIT_INCLUDE ${OCP_TOOLKIT_INCLUDE} ${OCP_ROOT} CACHE INTERNAL "")

message(STATUS log src "${OCP_FRAMEWORK_BACKEND_LOG_SRC}")
add_library(log_default)
add_library(OCP::Toolkit::Log::default ALIAS log_default)

target_sources(log_default PUBLIC log.h ${OCP_FRAMEWORK_BACKEND_LOG_SRC})
target_include_directories(log_default PUBLIC ${OCP_ROOT}/framework)

install(
  TARGETS log_default
  LIBRARY DESTINATION ./toolkit/backend/log/default/lib
  ARCHIVE DESTINATION ./toolkit/backend/log/default/lib
  RUNTIME DESTINATION ./toolkit/backend/log/default/bin
  INCLUDES
  DESTINATION ./toolkit/backend/log/default/include)
