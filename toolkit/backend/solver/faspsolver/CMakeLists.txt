add_subdirectory(external)

target_sources(solver PUBLIC solver.c solver.h ${OCP_FRAMEWORK_BACKEND_SOLVER_SRC})
target_include_directories(solver PUBLIC ${OCP_ROOT}/framework)
target_link_libraries(solver PUBLIC fasp)
