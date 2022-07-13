#pragma once

#include "build.hh"

namespace otter {
namespace mem {

class Allocator {
public:
  virtual ~Allocator() = default;

  /// Pointers returned by basic allocation functions must be aligned to `OTTER_MAX_ALIGN`.
  virtual void* alloc(usize size) = 0;
  virtual bool  dealloc(void* data, usize size) = 0;
  virtual void* realloc(void* data, usize size) = 0;

  /// If an allocator provides custom aligned allocation, all 3 functions must be implemented.
  virtual void* allocAligned(usize size, usize align);
  virtual bool  deallocAligned(void* data, usize size, usize align);
  virtual void* reallocAligned(void* data, usize size, usize align);
};

class OSAllocator : public Allocator {
public:
  void* alloc(usize size) override;
  bool  dealloc(void* data, usize size) override;
  void* realloc(void* data, usize size) override;
};

Allocator& getDefaultAllocator();

} // namespace mem
} // namespace otter

#define MEM_ALLOC(size)         (::otter::mem::getDefaultAllocator().alloc(size))
#define MEM_DEALLOC(data, size) (::otter::mem::getDefaultAllocator().dealloc(data, size))
#define MEM_REALLOC(data, size) (::otter::mem::getDefaultAllocator().realloc(data, size))

#define MEM_ALLOC_ALIGNED(size, align) \
  (::otter::mem::getDefaultAllocator().allocAligned(size, align))
#define MEM_DEALLOC_ALIGNED(data, size, align) \
  (::otter::mem::getDefaultAllocator().deallocAligned(data, size, align))
#define MEM_REALLOC_ALIGNED(data, size, align) \
  (::otter::mem::getDefaultAllocator().reallocAligned(data, size, align))
