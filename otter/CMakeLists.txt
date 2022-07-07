add_library(otter STATIC)
target_include_directories(otter PUBLIC include)

add_subdirectory(include/otter)
add_subdirectory(lib)
add_subdirectory(samples)
