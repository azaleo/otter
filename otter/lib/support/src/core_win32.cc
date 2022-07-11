#include "otter/support/core.hh"

#ifdef OTTER_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace otter {
namespace {

HANDLE getHeap() {
  static HANDLE Heap = GetProcessHeap();
  return Heap;
}

} // namespace

void* mem::alloc(usize bytes) {
  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapAlloc(heap, 0, bytes);
}

bool mem::dealloc(void* data, usize bytes) {
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  return HeapFree(heap, 0, data);
}

void* mem::realloc(void* data, usize bytes) {
  if (!data)
    return alloc(bytes);

  HANDLE heap = getHeap();
  if (!heap)
    return nullptr;

  return HeapReAlloc(heap, 0, data, bytes);
}

} // namespace otter
