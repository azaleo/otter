#include "otter/support/core.hh"
#include <stdlib.h>

#ifdef OTTER_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace otter {

void detail::fatal_error_impl(FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line,
                              bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

namespace {

#ifdef OTTER_WIN32
HANDLE get_heap() {
  static HANDLE Heap = GetProcessHeap();
  return Heap;
}
#endif

} // namespace

// TODO: Aligned alloc.

AllocBuffer OSAllocator::alloc(usize bytes, usize align) {
#ifdef OTTER_WIN32
  HANDLE heap = get_heap();
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
  HANDLE heap = get_heap();
  if (!heap)
    return {};

  void* new_data = HeapReAlloc(heap, 0, data, bytes);
  if (!new_data)
    return {};

  return {new_data, bytes};
#else
#error "missing OS realloc"
#endif
}

AllocResult OSAllocator::dealloc(void* data, usize bytes, usize align) {
#ifdef OTTER_WIN32
  HANDLE heap = get_heap();
  if (!heap)
    return {};

  auto res = HeapFree(heap, 0, data);
  return !res ? AllocResult_Success : AllocResult_SystemError;
#else
#error "missing OS dealloc"
#endif
}

namespace {

bool is_pow2(usize n) {
  return (n != 0) && (n & (n - 1)) == 0;
}

bool is_valid_align(usize n) {
  return (n > OTTER_MAX_ALIGN) && is_pow2(n);
}

} // namespace
} // namespace otter

void* otter::align(void* ptr, usize align) {
  ASSUME(is_valid_align(align));
  return (void*)(((usize)ptr & ~(align - 1)) + align);
}
