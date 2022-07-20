#pragma once

#include "build.hh"
#include "traits.hh"

namespace mem
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

template <typename T>
T* MEM_NEW() {
  return (T*)MEM_ALLOC_ALIGNED(sizeof(T), alignof(T));
}

template <typename T>
void MEM_DELETE(T* data) {
  if (!data)
    return;

  data->~T();
  MEM_DEALLOC_ALIGNED(data, sizeof(T), alignof(T));
}

namespace mem
{
  struct CustomNewType {};
  constexpr CustomNewType CustomNew;
}

inline void* operator new(usize, void* data, mem::CustomNewType) { return data; }
inline void  operator delete(void*, void*, mem::CustomNewType) {}

template <typename T, typename... Args>
void MEM_CONSTRUCT(T* data, Args&&... args) {
  new (data, mem::CustomNew) T(FORWARD(Args, args)...);
}
