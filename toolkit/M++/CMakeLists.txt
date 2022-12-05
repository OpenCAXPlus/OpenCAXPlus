add_subdirectory(external)

add_library(fem_Mpp)
add_library(OCP::Toolkit::M++ ALIAS fem_Mpp)

target_sources(fem_Mpp PUBLIC ${OCP_FRAMEWORK_BACKEND_DATA_SRC}
                              ${OCP_FRAMEWORK_BACKEND_CONTEXT_SRC})
target_include_directories(fem_Mpp PUBLIC ${OCP_ROOT}/framework)
target_link_libraries(fem_Mpp PUBLIC OCP::External::M++)
