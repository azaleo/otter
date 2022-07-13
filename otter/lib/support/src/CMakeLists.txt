target_sources(otter_support PRIVATE
  error_handling.cc
  memory.cc)

if (WIN32)
  target_sources(otter_support PRIVATE memory_win32.cc)
endif ()
