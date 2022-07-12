#pragma once

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

template <typename Int>
bool isPow2(Int n) {
  return n > 0 && (n & (n - 1)) == 0;
}

} // namespace math
} // namespace otter
