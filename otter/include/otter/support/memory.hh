#pragma once

#include "build.hh"

namespace otter
{
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
}

#define MEM_ALLOC(size)         (::otter::getDefaultAllocator().alloc(size))
#define MEM_DEALLOC(data, size) (::otter::getDefaultAllocator().dealloc(data, size))
#define MEM_REALLOC(data, size) (::otter::getDefaultAllocator().realloc(data, size))

#define MEM_ALLOC_ALIGNED(size, align) (::otter::getDefaultAllocator().allocAligned(size, align))
#define MEM_DEALLOC_ALIGNED(data, size, align) \
  (::otter::getDefaultAllocator().deallocAligned(data, size, align))
#define MEM_REALLOC_ALIGNED(data, size, align) \
  (::otter::getDefaultAllocator().reallocAligned(data, size, align))
