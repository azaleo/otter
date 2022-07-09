#include "otter/support/core.hh"
#include <stdlib.h>

namespace otter {

void detail::fatalErrorImpl(
    FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line, bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

void* Allocator::allocAligned(usize bytes, usize align) {
  // TODO.
  return alloc(bytes);
}

bool Allocator::deallocAligned(void* data, usize bytes, usize align) {
  // TODO.
  return dealloc(data, bytes);
}

void* Allocator::reallocAligned(void* data, usize bytes, usize align) {
  // TODO.
  return realloc(data, bytes);
}

} // namespace otter
