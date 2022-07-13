#pragma once

#include "build.hh"

namespace otter {
namespace traits {

template <typename T>
struct TypeIdentity {
  using Type = T;
};

template <typename T, T V>
struct ValueIdentity {
  static constexpr T Value = V;
};

template <bool B>
struct BoolValue : ValueIdentity<bool, B> {};

struct TrueType : BoolValue<true> {};
struct FalseType : BoolValue<false> {};

template <bool B, bool... Bs>
struct Any : BoolValue<B || Any<Bs...>::Value> {};
template <bool B>
struct Any<B> : BoolValue<B> {};

template <bool B, bool... Bs>
struct All : BoolValue<B && All<Bs...>::Value> {};
template <bool B>
struct All<B> : BoolValue<B> {};

template <typename A, typename B>
struct IsSame : FalseType {};
template <typename T>
struct IsSame<T, T> : TrueType {};

template <typename T, typename... Ts>
struct IsAny : Any<IsSame<T, Ts>::Value...> {};

template <typename T>
struct IsInt : IsAny<T, i8, i16, i32, i64, u8, u16, u32, u64> {};
template <typename T>
struct IsFloat : IsAny<T, f32, f64> {};

} // namespace traits
} // namespace otter
