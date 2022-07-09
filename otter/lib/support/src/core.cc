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

// TODO: Aligned alloc.

AllocBuffer OSAllocator::alloc(usize bytes, usize align) {
#ifdef OTTER_WIN32
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  void* data = HeapAlloc(heap, 0, bytes);
  if (!data)
    return {};

  return {data, bytes};
#else
#error "missing OS alloc impl"
#endif
}

AllocBuffer OSAllocator::realloc(void* data, usize bytes, usize align) {
#ifdef OTTER_WIN32
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  void* newData = HeapReAlloc(heap, 0, data, bytes);
  if (!newData)
    return {};

  return {newData, bytes};
#else
#error "missing OS realloc"
#endif
}

AllocResult OSAllocator::dealloc(void* data, usize bytes, usize align) {
#ifdef OTTER_WIN32
  HANDLE heap = getHeap();
  if (!heap)
    return {};

  auto res = HeapFree(heap, 0, data);
  return !res ? AllocResult_Success : AllocResult_SystemError;
#else
#error "missing OS dealloc"
#endif
}

namespace {

bool isPow2(usize n) {
  return n != 0 && (n & (n - 1)) == 0;
}

bool isValidAlign(usize n) {
  return n > OTTER_MAX_ALIGN && isPow2(n);
}

} // namespace
} // namespace otter

void* otter::align(void* ptr, usize align) {
  ASSUME(isValidAlign(align));
  return (void*)(((usize)ptr & ~(align - 1)) + align);
}
