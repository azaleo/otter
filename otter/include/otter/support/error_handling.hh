#pragma once

#include "build.hh"

#ifdef OTTER_DEBUG
#define OTTER_DEBUG_FATAL_ERRORS true
#else
#define OTTER_DEBUG_FATAL_ERRORS false
#endif

enum FatalError {
  FatalError_AssertFailed,
  FatalError_BadAlloc,
  FatalError_Unreachable,
};

namespace detail
{
  template <usize N>
  usize findCharArrayLength(const char (&arr)[N]) {
    if (!N)
      return 0;

    return arr[N - 1] == '\0' ? N - 1 : N;
  }

  struct CoreStringSpan {
    const char* Data = nullptr;
    usize       Length = 0;

    template <usize N>
    /*implicit*/ CoreStringSpan(const char (&arr)[N])
        : Data(arr)
        , Length(findCharArrayLength(arr)) {}
  };

  OTTER_NORETURN void fatalErrorImpl(
      FatalError     err,
      CoreStringSpan msg,
      CoreStringSpan file,
      i32            line,
      bool           debug = OTTER_DEBUG_FATAL_ERRORS);

  inline void assertImpl(
      bool           pred,
      CoreStringSpan msg,
      CoreStringSpan file,
      i32            line,
      bool           debug = OTTER_DEBUG_FATAL_ERRORS) {
    if (!pred)
      fatalErrorImpl(FatalError_AssertFailed, msg, file, line, debug);
  }
}

#define OTTER_FATAL_ERROR(err, msg) (::detail::fatalErrorImpl(err, msg, __FILE__, __LINE__))

#define ASSERT(pred, ...) \
  (::detail::assertImpl(pred, "assert failed: \"" #pred "\"", __FILE__, __LINE__))

#ifdef OTTER_DEBUG
#define ASSUME(pred, ...) ASSERT(pred, __VA_ARGS__)
#else
#define ASSUME(pred, ...) ((void)0)
#endif

#define BAD_ALLOC(msg)   OTTER_FATAL_ERROR(::FatalError_BadAlloc, msg)
#define UNREACHABLE(msg) OTTER_FATAL_ERROR(::FatalError_Unreachable, msg)
