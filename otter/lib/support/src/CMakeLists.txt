target_sources(otter_support PRIVATE core.cc)

if (WIN32)
  target_sources(otter_support PRIVATE core_win32.cc)
endif ()
