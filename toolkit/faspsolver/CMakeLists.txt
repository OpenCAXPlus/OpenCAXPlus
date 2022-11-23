add_subdirectory(external)

add_library(solver_faspsolver)
add_library(OCP::Toolkit::faspsolver ALIAS solver_faspsolver)

target_sources(solver_faspsolver PUBLIC solver.c solver.h
                                        ${OCP_FRAMEWORK_BACKEND_SOLVER_SRC})
target_include_directories(
  solver_faspsolver PUBLIC ${OCP_ROOT}/framework
                           ${FASP_SOURCE_DIR}/base/include)
target_link_libraries(solver_faspsolver PUBLIC fasp m)

# install( TARGETS solver_faspsolver LIBRARY DESTINATION
# ./toolkit/backend/solver/faspsolver/lib ARCHIVE DESTINATION
# ./toolkit/backend/solver/faspsolver/lib RUNTIME DESTINATION
# ./toolkit/backend/solver/faspsolver/bin INCLUDES DESTINATION
# ./toolkit/backend/solver/faspsolver/include)

install(
  DIRECTORY .
  DESTINATION toolkit/faspsolver
  COMPONENT ${PROJECT_NAME}
  PATTERN "external" EXCLUDE)
