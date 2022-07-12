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

} // namespace math
} // namespace otter
