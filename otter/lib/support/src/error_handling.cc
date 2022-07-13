#include "otter/support/error_handling.hh"
#include <stdlib.h>

namespace otter {
namespace err {

void detail::fatalErrorImpl(
    FatalError err, CoreStringSpan msg, CoreStringSpan file, i32 line, bool debug) {
  exit(err == FatalError_BadAlloc ? 102 : 101);
}

} // namespace err
} // namespace otter
