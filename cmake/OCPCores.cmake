include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CORES_TO_USE ${N})
else()
  set(CORES_TO_USE 1)
endif()

# Calculate half of the available cores
math(EXPR HALF_CORES "${CORES_TO_USE} / 2")
message(STATUS "Using ${HALF_CORES} cores for building")

# Set the build flags to use half of the available cores
if (MSVC)
  add_compile_options(/MP${HALF_CORES})
else()
  set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${HALF_CORES}")
endif()