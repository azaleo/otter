#include "otter/support/core.hh"
#include <stdlib.h>

namespace otter {

void detail::fatalErrorImpl(
    FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line, bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

namespace {

bool isPow2(usize n) {
  return n != 0 && (n & (n - 1)) == 0;
}

} // namespace

void* mem::align(void* ptr, usize align) {
  ASSUME(isPow2(align));
  return (void*)(((usize)ptr & ~(align - 1)) + align);
}

namespace {

usize getTotalAlignedBufSize(usize baseSize, usize align) {
  ASSUME(isPow2(align));
  ASSUME(align > OTTER_MAX_ALIGN);

  // Worst case scenario, the base pointer needs to be offset by (align - 1).
  return max(baseSize + (align - 1), sizeof(void*));
}

} // namespace

void* mem::allocAligned(usize size, usize align, Allocator& al) {
  if (align <= OTTER_MAX_ALIGN)
    return al.alloc(size);

  ASSUME(isPow2(align));

  void* basePtr = al.alloc(getTotalAlignedBufSize(size, align));
  void* alignedPtr = mem::align((void**)basePtr + 1, align);

  *((void**)alignedPtr - 1) = basePtr;
  return alignedPtr;
}

bool mem::deallocAligned(void* data, usize size, usize align, Allocator& al) {
  if (!data) {
    // Return early to avoid de-referencing nullptr.
    return true;
  }

  if (align <= OTTER_MAX_ALIGN)
    return al.dealloc(data, size);

  ASSUME(isPow2(align));
  ASSUME((usize)data % align == 0);

  void* basePtr = *((void**)data - 1);
  return al.dealloc(basePtr, size);
}

void* mem::reallocAligned(void* data, usize size, usize align, Allocator& al) {
  if (!data)
    return allocAligned(size, align, al);

  if (align <= OTTER_MAX_ALIGN)
    return al.realloc(data, size);

  ASSUME(isPow2(align));
  ASSUME((usize)data % align == 0);

  void* basePtr = *((void**)data - 1);
  void* newBasePtr = al.alloc(getTotalAlignedBufSize(size, align));
  void* alignedPtr = mem::align((void**)newBasePtr + 1, align);

  *((void**)alignedPtr - 1) = basePtr;
  return alignedPtr;
}

} // namespace otter
