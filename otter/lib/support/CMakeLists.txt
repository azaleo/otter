add_library(otter_support STATIC)
target_link_libraries(otter PUBLIC otter_support)

target_include_directories(otter_support
  PUBLIC ${OTTER_INCLUDE_DIRS}
  PRIVATE include)

add_subdirectory(include/otter_support)
add_subdirectory(src)
