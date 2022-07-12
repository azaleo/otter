#pragma once

#ifndef NDEBUG
#define OTTER_DEBUG
#endif

#ifdef _MSC_VER
#define OTTER_MSVC
#else
#error "unsupported compiler"
#endif

#ifdef _WIN32
#define OTTER_WIN32
#else
#error "unupported OS"
#endif

#ifdef _M_X64
#define OTTER_X86_64
#else
#error "unsupported arch"
#endif

#ifdef OTTER_X86_64
#define OTTER_64_BITS
#else
#error "unsupported pointer width"
#endif

#if defined(OTTER_WIN32) && defined(OTTER_64_BITS)
#define OTTER_LLP64
#else
#error "unsupported data model"
#endif

#ifdef OTTER_MSVC
#define OTTER_NORETURN __declspec(noreturn)
#else
#error "missing compiler-specific attribs"
#endif

#if defined(OTTER_WIN32) && defined(OTTER_64_BITS)
#define OTTER_MAX_ALIGN (16)
#else
#error "missing max align"
#endif

#ifdef OTTER_LLP64
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
#else
#error "missing integer types"
#endif

#ifdef OTTER_64_BITS
typedef i64 isize;
typedef u64 usize;
#else
#error "missing size types"
#endif

#ifdef OTTER_MSVC
typedef float  f32;
typedef double f64;
#else
#error "missing floating-point types"
#endif

namespace otter {
namespace err {

enum FatalError
{
  FatalError_AssertFailed,
  FatalError_BadAlloc,
  FatalError_Unreachable,
};

namespace detail {

template <usize N>
usize findCharArrayLength(const char (&arr)[N]) {
  if (!N)
    return 0;

  return arr[N - 1] == '\0' ? N - 1 : N;
}

struct CoreStringSpan
{
  const char* _data = nullptr;
  usize       _length = 0;

  template <usize N>
  /*implicit*/ CoreStringSpan(const char (&arr)[N])
      : _data(arr)
      , _length(findCharArrayLength(arr)) {}
};

#ifdef OTTER_DEBUG
#define ERR_DEBUG_FATAL_ERRORS true
#else
#define ERR_DEBUG_FATAL_ERRORS false
#endif

OTTER_NORETURN void fatalErrorImpl(
    FatalError     err,
    CoreStringSpan msg,
    CoreStringSpan file,
    i32            line,
    bool           debug = ERR_DEBUG_FATAL_ERRORS);

inline void assertImpl(
    bool           pred,
    CoreStringSpan msg,
    CoreStringSpan file,
    i32            line,
    bool           debug = ERR_DEBUG_FATAL_ERRORS) {
  if (!pred)
    fatalErrorImpl(FatalError_AssertFailed, msg, file, line, debug);
}

} // namespace detail
} // namespace err
} // namespace otter

#define ERR_FATAL_ERROR(err, msg) \
  (::otter::err::detail::fatalErrorImpl(err, msg, __FILE__, __LINE__))

#define ASSERT(pred, ...) \
  (::otter::err::detail::assertImpl(pred, "assert failed: \"" #pred "\"", __FILE__, __LINE__))

#ifdef OTTER_DEBUG
#define ASSUME(pred, ...) ASSERT(pred, __VA_ARGS__)
#else
#define ASSUME(pred, ...) ((void)0)
#endif

#define ERR_BAD_ALLOC(msg)   ERR_FATAL_ERROR(::otter::err::FatalError_BadAlloc, msg)
#define ERR_UNREACHABLE(msg) ERR_FATAL_ERROR(::otter::err::FatalError_Unreachable, msg)

namespace otter {
namespace mem {

struct Allocator
{
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

struct OSAllocator : Allocator
{
  void* alloc(usize size) override;
  bool  dealloc(void* data, usize size) override;
  void* realloc(void* data, usize size) override;
};

inline Allocator& getDefaultAllocator() {
  static OSAllocator _instance;
  return _instance;
}

/// \pre \p align is assumed to be a power of 2.
void* align(void* ptr, usize align);

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
