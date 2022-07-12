#pragma once

#include "traits.hh"

namespace otter {
namespace math {

template <typename T>
const T& min(const T& a, const T& b) {
  return a < b ? a : b;
}

template <typename T>
const T& max(const T& a, const T& b) {
  return a > b ? a : b;
}

template <typename T>
const T& clamp(const T& val, const T& a, const T& b) {
  return min(max(a, val), b);
}

template <typename T>
bool isPow2(T n) {
  static_assert(traits::IsInt<T>::Value, "`isPow2` only accepts integers");
  return n > 0 && (n & (n - 1)) == 0;
}

} // namespace math
} // namespace otter
