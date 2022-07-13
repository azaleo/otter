#include "otter/support/error_handling.hh"
#include <stdlib.h>

namespace otter
{
  void detail::fatalErrorImpl(
      FatalError::Enum err, CoreStringSpan msg, CoreStringSpan file, i32 line, bool debug) {
    exit(err == FatalError::BadAlloc ? 102 : 101);
  }
}
