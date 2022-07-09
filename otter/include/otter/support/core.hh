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

#define UNUSED(...) (void)(__VA_ARGS__)

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

class CoreStringSpan
{
  const char* Data = nullptr;
  usize       Length = 0;

public:
  template <usize N>
  /*implicit*/ CoreStringSpan(const char (&arr)[N])
    : Data(arr)
    , Length(findCharArrayLength(arr)) {}

  const char* data() const { return Data; }
  usize       length() const { return Length; }
};

#ifdef OTTER_DEBUG
#define OTTER_DEBUG_FATAL_ERRORS true
#else
#define OTTER_DEBUG_FATAL_ERRORS false
#endif

OTTER_NORETURN void fatalErrorImpl(FatalError err, CoreStringSpan msg, CoreStringSpan file,
                                   i32 line, bool debug = OTTER_DEBUG_FATAL_ERRORS);

inline void assertImpl(bool pred, CoreStringSpan msg, CoreStringSpan file, i32 line,
                       bool debug = OTTER_DEBUG_FATAL_ERRORS) {
  if (!pred)
    fatalErrorImpl(FatalError_AssertFailed, msg, file, line, debug);
}

} // namespace detail
} // namespace otter

#define OTTER_FATAL_ERROR(err, msg) (::otter::detail::fatalErrorImpl(err, msg, __FILE__, __LINE__))
#define ASSERT(pred, ...) \
  (::otter::detail::assertImpl(pred, "assert failed: \"" #pred "\"", __FILE__, __LINE__))

#ifdef OTTER_DEBUG
#define ASSUME(pred, ...) ASSERT(pred, __VA_ARGS__)
#else
#define ASSUME(pred, ...) (void)0
#endif

#define BAD_ALLOC(msg)   OTTER_FATAL_ERROR(::otter::FatalError_BadAlloc, msg)
#define UNREACHABLE(msg) OTTER_FATAL_ERROR(::otter::FatalError_Unreachable, msg)

#if defined(OTTER_WIN32) && defined(OTTER_64_BITS)
#define OTTER_MAX_ALIGN 16
#else
#error "missing max align"
#endif

namespace otter {

enum AllocResult
{
  AllocResult_Success = 0,
  AllocResult_SystemError,
};

struct AllocBuffer
{
  void* Data = nullptr;
  usize Size = 0;

  bool isValid() const { return Data; }
  bool operator!() const { return !isValid(); }
};

class Allocator
{
public:
  virtual ~Allocator() = default;

  /// Returned buffers must be aligned to at least `OTTER_MAX_ALIGN`.
  virtual AllocBuffer alloc(usize bytes, usize align) = 0;
  virtual AllocBuffer realloc(void* data, usize bytes, usize align) = 0;
  virtual AllocResult dealloc(void* data, usize bytes, usize align) = 0;
};

class OSAllocator : public Allocator
{
public:
  AllocBuffer alloc(usize bytes, usize align) override;
  AllocBuffer realloc(void* data, usize bytes, usize align) override;
  AllocResult dealloc(void* data, usize bytes, usize align) override;
};

inline Allocator& getDefaultAllocator() {
  static OSAllocator Alloc;
  return Alloc;
}

struct NewType
{
};
constexpr NewType New;

} // namespace otter

inline void* operator new(usize bytes, usize align, otter::NewType) {
  using namespace otter;

  auto&       al = getDefaultAllocator();
  AllocBuffer buf = al.alloc(bytes, align);
  if (!buf)
    BAD_ALLOC("could not alloc with operator new");

  return buf.Data;
}

inline void* operator new(usize, void* data, otter::NewType) {
  return data;
}

inline void operator delete(void* data, usize bytes, usize align, otter::NewType) {
  auto& al = otter::getDefaultAllocator();
  if (auto err = al.dealloc(data, bytes, align)) {
    UNUSED(err);
    BAD_ALLOC("could not dealloc with operator delete");
  }
}

inline void operator delete(void*, void*, otter::NewType) {}

#define OTTER_NEW(type)          new (alignof(type), otter::New) type
#define OTTER_DELETE(type, data) operator delete(data, sizeof(type), alignof(type), otter::New)
