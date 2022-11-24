# spdlog 1.11.0
add_subdirectory(external)

# set(OCP_TOOLKIT_SRC ${OCP_TOOLKIT_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/log.h CACHE INTERNAL "")

# set(OCP_TOOLKIT_LINK ${OCP_TOOLKIT_LINK} spdlog::spdlog CACHE INTERNAL "")
add_library(log_spdlog)
add_library(OCP::Toolkit::spdlog ALIAS log_spdlog)

target_sources(log_spdlog PUBLIC log.cpp log.h ${OCP_FRAMEWORK_BACKEND_LOG_SRC})
target_include_directories(log_spdlog PUBLIC ${OCP_ROOT}/framework)
target_link_libraries(log_spdlog PUBLIC OCP::External::spdlog)

# install( TARGETS log_spdlog LIBRARY DESTINATION
# ./toolkit/backend/log/spdlog/lib ARCHIVE DESTINATION
# ./toolkit/backend/log/spdlog/lib RUNTIME DESTINATION
# ./toolkit/backend/log/spdlog/bin INCLUDES DESTINATION
# ./toolkit/backend/log/spdlog/include)

install(
  DIRECTORY .
  DESTINATION toolkit/spdlog
  COMPONENT ${PROJECT_NAME}
  PATTERN "external" EXCLUDE)
