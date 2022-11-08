add_library(frontend STATIC)
add_library(OCP::frontend ALIAS frontend)

add_subdirectory(visualization)
