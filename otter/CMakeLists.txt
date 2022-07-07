add_library(otter STATIC)

set(OTTER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(otter PUBLIC ${OTTER_INCLUDE_DIRS})

add_subdirectory(include/otter)
add_subdirectory(lib)
add_subdirectory(samples)
