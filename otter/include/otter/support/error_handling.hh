#pragma once

#include "build.hh"

namespace otter {
namespace err {

enum FatalError {
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

class CoreStringSpan {
  const char* _data = nullptr;
  usize       _length = 0;

public:
  template <usize N>
  /*implicit*/ CoreStringSpan(const char (&arr)[N])
      : _data(arr)
      , _length(findCharArrayLength(arr)) {}

  const char* data() const { return _data; }
  usize       length() const { return _length; }
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
