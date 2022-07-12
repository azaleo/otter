#include "otter/support/core.hh"
#include "otter/support/math.hh"
#include <stdlib.h>

namespace otter {
namespace err {

void detail::fatalErrorImpl(
    FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line, bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

} // namespace err

namespace mem {
namespace {

bool isPow2(usize n) {
  return n != 0 && (n & (n - 1)) == 0;
}

usize getTotalAlignedBufSize(usize size, usize align) {
  ASSUME(isPow2(align));
  ASSUME(align > OTTER_MAX_ALIGN);

  // Worst case scenario, the base pointer needs to be offset by (align - OTTER_MAX_ALIGN).
  // This assumes base pointers are always aligned to OTTER_MAX_ALIGN (which is a requirement of the
  // Allocator interface).
  return math::max(size + (align - OTTER_MAX_ALIGN), sizeof(void*));
}

void* align(void* basePtr, usize align) {
  ASSUME(isPow2(align));
  ASSUME(align > OTTER_MAX_ALIGN);
  ASSUME((usize)basePtr % OTTER_MAX_ALIGN == 0);

  // Make sure there is room to store the base ptr.
  void* start = (void**)basePtr + 1;
  void* aligned = (void*)(((usize)start & ~(align - 1)) + align);

  *((void**)aligned - 1) = basePtr;
  return aligned;
}

} // namespace

void* Allocator::allocAligned(usize size, usize align) {
  if (align <= OTTER_MAX_ALIGN)
    return alloc(size);

  ASSUME(isPow2(align));

  void* basePtr = alloc(getTotalAlignedBufSize(size, align));
  if (!basePtr)
    return nullptr;

  return mem::align(basePtr, align);
}

bool Allocator::deallocAligned(void* data, usize size, usize align) {
  if (!data) {
    // Return early to avoid de-referencing nullptr.
    return true;
  }

  if (align <= OTTER_MAX_ALIGN)
    return dealloc(data, size);

  ASSUME(isPow2(align));
  ASSUME((usize)data % align == 0);

  void* basePtr = *((void**)data - 1);
  return dealloc(basePtr, getTotalAlignedBufSize(size, align));
}

void* Allocator::reallocAligned(void* data, usize size, usize align) {
  if (!data)
    return allocAligned(size, align);

  if (align <= OTTER_MAX_ALIGN)
    return realloc(data, size);

  ASSUME(isPow2(align));
  ASSUME((usize)data % align == 0);

  void* oldBasePtr = *((void**)data - 1);
  void* basePtr = realloc(oldBasePtr, getTotalAlignedBufSize(size, align));
  if (!basePtr)
    return nullptr;

  return mem::align(basePtr, align);
}

} // namespace mem
} // namespace otter
