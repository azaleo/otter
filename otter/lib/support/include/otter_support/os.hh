#pragma once

#include "otter/support/build.hh"
#include "otter/support/memory.hh"

namespace detail
{
  namespace os
  {
    void* alloc(usize size);
    bool  dealloc(void* data, usize size);
    void* realloc(void* data, usize size);
  }
}
