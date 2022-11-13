# spdlog 1.11.0
add_subdirectory(external)

# set(OCP_TOOLKIT_SRC ${OCP_TOOLKIT_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/log.h CACHE INTERNAL "")

# set(OCP_TOOLKIT_LINK ${OCP_TOOLKIT_LINK} spdlog::spdlog CACHE INTERNAL "")

target_sources(log PUBLIC log.cpp log.h ${OCP_FRAMEWORK_BACKEND_LOG_SRC})
target_include_directories(log PUBLIC ${OCP_ROOT}/framework)
target_link_libraries(log PUBLIC spdlog::spdlog)
