#include "otter/support/memory.hh"

#ifdef OTTER_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace otter {
namespace mem {
namespace {

HANDLE getHeap() {
  static HANDLE Heap = GetProcessHeap();
  return Heap;
}

} // namespace

void* OSAllocator::alloc(usize bytes) {
  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapAlloc(heap, 0, bytes);
}

bool OSAllocator::dealloc(void* data, usize) {
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  return HeapFree(heap, 0, data);
}

void* OSAllocator::realloc(void* data, usize bytes) {
  if (!data)
    return alloc(bytes);

  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapReAlloc(heap, 0, data, bytes);
}

} // namespace mem
} // namespace otter
