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

enum FatalError {
  FatalError_AssertFailed,
  FatalError_BadAlloc,
  FatalError_Unreachable,
};

namespace detail {

template <usize N>
usize findRealCharArrayLength(const char (&arr)[N]) {
  if (!N)
    return 0;
  return arr[N - 1] == '\0' ? N - 1 : N;
}

class CoreStringSpan {
  const char* Data = nullptr;
  usize       Length = 0;

public:
  template <usize N>
  /*implicit*/ CoreStringSpan(const char (&arr)[N])
    : Data(arr)
    , Length(findRealCharArrayLength(arr)) {}

  const char* data() const { return Data; }
  usize       length() const { return Length; }
};

#ifdef OTTER_DEBUG
#define OTTER_ENABLE_FATAL_ERROR_DEBUG true
#else
#define OTTER_ENABLE_FATAL_ERROR_DEBUG false
#endif

OTTER_NORETURN void
raiseFatalErrorImpl(FatalError     err,
                    CoreStringSpan msg,
                    CoreStringSpan file,
                    i32            line,
                    bool           debug = OTTER_ENABLE_FATAL_ERROR_DEBUG);

inline void assertImpl(bool           pred,
                       CoreStringSpan msg,
                       CoreStringSpan file,
                       i32            line,
                       bool           debug = OTTER_ENABLE_FATAL_ERROR_DEBUG) {
  if (!pred)
    raiseFatalErrorImpl(FatalError_AssertFailed, msg, file, line, debug);
}

} // namespace detail
} // namespace otter

#define OTTER_RAISE_FATAL_ERROR(err, msg) \
  (::otter::detail::raiseFatalErrorImpl(err, msg, __FILE__, __LINE__))

#define ASSERT(pred, ...) \
  (::otter::detail::assertImpl( \
    pred, "assert failed: \"" #pred "\"", __FILE__, __LINE__))

#ifdef OTTER_DEBUG
#define ASSUME(pred, ...) ASSERT(pred, __VA_ARGS__)
#else
#define ASSUME(pred, ...) UNUSED(0)
#endif

#define BAD_ALLOC(msg) \
  OTTER_RAISE_FATAL_ERROR(::otter::FatalError_BadAlloc, msg)
#define UNREACHABLE(msg) \
  OTTER_RAISE_FATAL_ERROR(::otter::FatalError_Unreachable, msg)
