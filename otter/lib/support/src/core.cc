#include "otter/support/core.hh"
#include <stdlib.h>

#ifdef OTTER_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace otter {

void detail::fatalErrorImpl(FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line,
                            bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

#ifdef OTTER_WIN32
namespace {

HANDLE getHeap() {
  static HANDLE Heap = GetProcessHeap();
  return Heap;
}

} // namespace
#endif

void* Allocator::allocAligned(usize bytes, usize align) {
  // TODO: aligned alloc.
  return alloc(bytes);
}

bool Allocator::deallocAligned(void* data, usize bytes, usize align) {
  return dealloc(data, bytes);
}

void* Allocator::reallocAligned(void* data, usize bytes, usize align) {
  return realloc(data, bytes);
}

void* OSAllocator::alloc(usize bytes) {
#ifdef OTTER_WIN32
  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapAlloc(heap, 0, bytes);
#else
#error "missing OS alloc impl"
#endif
}

bool OSAllocator::dealloc(void* data, usize bytes) {
#ifdef OTTER_WIN32
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  return HeapFree(heap, 0, data);
#else
#error "missing OS dealloc impl"
#endif
}

void* OSAllocator::realloc(void* data, usize bytes) {
#ifdef OTTER_WIN32
  if (!data)
    return alloc(bytes);

  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapReAlloc(heap, 0, data, bytes);
#else
#error "missing OS realloc impl"
#endif
}

} // namespace otter
