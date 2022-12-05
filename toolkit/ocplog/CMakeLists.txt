set(libname ocplog)

add_library(${libname})
add_library(OCP::Toolkit::${libname} ALIAS ${libname})
target_sources(${libname} PUBLIC log.c log.h)
target_link_libraries(${libname} PUBLIC OCP::Framework::Interface)
# target_include_directories(${libname} PUBLIC ${OCP_ROOT}/framework)

install(
  TARGETS ${libname}
  LIBRARY DESTINATION toolkit/ocplog/lib
  ARCHIVE DESTINATION toolkit/ocplog/lib
  RUNTIME DESTINATION toolkit/ocplog/bin
  INCLUDES
  DESTINATION toolkit/ocplog/include
  COMPONENT ${PROJECT_NAME})

# install(
#   DIRECTORY .
#   DESTINATION toolkit/ocplog
#   COMPONENT ${PROJECT_NAME})
