#pragma once

#include "build.hh"

namespace mem
{
  struct Allocator {
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

  Allocator& getDefaultAllocator();
}

#define MEM_ALLOC(size)         (::mem::getDefaultAllocator().alloc(size))
#define MEM_DEALLOC(data, size) (::mem::getDefaultAllocator().dealloc(data, size))
#define MEM_REALLOC(data, size) (::mem::getDefaultAllocator().realloc(data, size))

#define MEM_ALLOC_ALIGNED(size, align) (::mem::getDefaultAllocator().allocAligned(size, align))
#define MEM_DEALLOC_ALIGNED(data, size, align) \
  (::mem::getDefaultAllocator().deallocAligned(data, size, align))
#define MEM_REALLOC_ALIGNED(data, size, align) \
  (::mem::getDefaultAllocator().reallocAligned(data, size, align))

namespace mem
{
  struct CustomNewType {};
  constexpr CustomNewType CustomNew;
}

inline void* operator new(usize, void* data, mem::CustomNewType) { return data; }
inline void  operator delete(void*, void*, mem::CustomNewType) {}

namespace mem
{
  template <typename T, typename... Args>
  void construct(T* data, Args&&... args) {
    new (data, CustomNew) T(args...);
  }
}

#define MEM_CONSTRUCT(data, ...) (::mem::construct(data, __VA_ARGS__))
