# add_subdirectory(backend)
# add_subdirectory(frontend) add_subdirectory(communication)
# add_subdirectory(pbs)

# set(OCP_SDK_SRC
#     ${OCP_SDK_SRC} ${OCP_FRAMEWORK_SRC}
#     CACHE INTERNAL "")

add_library(framework INTERFACE)
add_library(OCP::Framework::Interface ALIAS framework)
target_include_directories(framework INTERFACE ${OCP_ROOT}/framework)
