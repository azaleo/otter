#pragma once

#include "build.hh"

namespace traits
{
  template <typename T>
  struct TypeIdentity {
    using Type = T;
  };

  template <typename T, T V>
  struct ValueIdentity {
    static constexpr T Value = V;
  };

  template <typename T>
  struct RemoveRef : TypeIdentity<T> {};
  template <typename T>
  struct RemoveRef<T&> : TypeIdentity<T> {};
  template <typename T>
  struct RemoveRef<T&&> : TypeIdentity<T> {};

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

  template <typename T>
  struct IsLValueRef : FalseType {};
  template <typename T>
  struct IsLValueRef<T&> : TrueType {};

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

  template <typename T>
  typename RemoveRef<T>::Type&& move(T&& val) {
    using Ret = typename RemoveRef<T>::Type&&;
    return (Ret)val;
  }

  template <typename T>
  T&& forward(typename RemoveRef<T>::Type& val) {
    // (For some reason, formatting breaks when casting to the type directly).
    using Ret = T&&;
    return (Ret)val;
  }

  template <typename T>
  T&& forward(typename RemoveRef<T>::Type&& val) {
    // See: https://en.cppreference.com/w/cpp/utility/forward.
    static_assert(!IsLValueRef<T>::Value, "can't forward rvalue as lvalue");

    using Ret = T&&;
    return (Ret)val;
  }
}

#define MOVE(val)          (::traits::move(val))
#define FORWARD(type, val) (::traits::forward<type>(val))
