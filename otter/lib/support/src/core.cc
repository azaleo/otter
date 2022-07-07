#include "otter/support/core.hh"
#include <stdlib.h>

namespace otter {

void detail::raiseFatalErrorImpl(FatalError     err,
                                 CoreStringSpan msg,
                                 CoreStringSpan file,
                                 i32            line,
                                 bool           debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

} // namespace otter
