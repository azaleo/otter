#pragma once

#include "build.hh"

#ifdef OTTER_DEBUG
#define DEBUG_FATAL_ERRORS true
#else
#define DEBUG_FATAL_ERRORS false
#endif

namespace otter
{
  namespace FatalError
  {
    enum Enum {
      AssertFailed,
      BadAlloc,
      Unreachable,
    };
  }

  namespace detail
  {
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

    OTTER_NORETURN void fatalErrorImpl(
        FatalError::Enum err,
        CoreStringSpan   msg,
        CoreStringSpan   file,
        i32              line,
        bool             debug = DEBUG_FATAL_ERRORS);

    inline void assertImpl(
        bool           pred,
        CoreStringSpan msg,
        CoreStringSpan file,
        i32            line,
        bool           debug = DEBUG_FATAL_ERRORS) {
      if (!pred)
        fatalErrorImpl(FatalError::AssertFailed, msg, file, line, debug);
    }
  }
}

#define OTTER_FATAL_ERROR(err, msg) (::otter::detail::fatalErrorImpl(err, msg, __FILE__, __LINE__))

#define ASSERT(pred, ...) \
  (::otter::detail::assertImpl(pred, "assert failed: \"" #pred "\"", __FILE__, __LINE__))

#ifdef OTTER_DEBUG
#define ASSUME(pred, ...) ASSERT(pred, __VA_ARGS__)
#else
#define ASSUME(pred, ...) ((void)0)
#endif

#define BAD_ALLOC(msg)   OTTER_FATAL_ERROR(::otter::FatalError::BadAlloc, msg)
#define UNREACHABLE(msg) OTTER_FATAL_ERROR(::otter::FatalError::Unreachable, msg)
