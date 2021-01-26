// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PARAMETER_PACK_H_
#define BASE_PARAMETER_PACK_H_

#include <stddef.h>

#include <initializer_list>
#include <tuple>
#include <type_traits>

#include "base/template_util.h"
#include "build/build_config.h"

namespace base {

// Checks if any of the elements in |ilist| is true.
// Similar to std::any_of for the case of constexpr initializer_list.
inline constexpr bool any_of(std::initializer_list<bool> ilist) {
  for (auto c : ilist) {
    if (c)
      return true;
  }
  return false;
}

// Checks if all of the elements in |ilist| are true.
// Similar to std::all_of for the case of constexpr initializer_list.
inline constexpr bool all_of(std::initializer_list<bool> ilist) {
  for (auto c : ilist) {
    if (!c)
      return false;
  }
  return true;
}

// Counts the elements in |ilist| that are equal to |value|.
// Similar to std::count for the case of constexpr initializer_list.
template <class T>
inline constexpr size_t count(std::initializer_list<T> ilist, T value) {
  size_t c = 0;
  for (const auto& v : ilist) {
    c += (v == value);
  }
  return c;
}

template <class... Ts >
struct if_all;

template <>
struct if_all<>
    : std::integral_constant<bool, true> {};

template <class T, class... Ts >
struct if_all<T, Ts...>
    : std::conditional<T::value, if_all<Ts...>, std::integral_constant<bool, false>>::type {};


template <class... Ts >
struct if_any;

template <>
struct if_any<>
    : std::integral_constant<bool, false> {};

template <class T, class... Ts >
struct if_any<T, Ts...>
    : std::conditional<T::value, std::integral_constant<bool, true>, if_any<Ts...>>::type {};

constexpr size_t pack_npos = -1;

template <typename... Ts>
struct ParameterPack {
  // Checks if |Type| occurs in the parameter pack.
  template <typename Type>
  using HasType = bool_constant<if_any<std::is_same<Type, Ts>...>::value>;

  // Checks if the parameter pack only contains |Type|.
  template <typename Type>
  using OnlyHasType = bool_constant<if_all<std::is_same<Type, Ts>...>::value>;

  // Breaks build with MSVC 2017 but it is not used.
#if !defined(COMPILER_MSVC)
  // Checks if |Type| occurs only once in the parameter pack.
  template <typename Type>
  using IsUniqueInPack =
      bool_constant<count({std::is_same<Type, Ts>::value...}, true) == 1>;
#endif

  // Returns the zero-based index of |Type| within |Pack...| or |pack_npos| if
  // it's not within the pack.
  template <typename Type>
  static constexpr size_t IndexInPack() {
    size_t index = 0;
    for (bool value : {std::is_same<Type, Ts>::value...}) {
      if (value)
        return index;
      index++;
    }
    return pack_npos;
  }

  // Helper for extracting the Nth type from a parameter pack.
  template <size_t N>
  using NthType = std::tuple_element_t<N, std::tuple<Ts...>>;

  // Checks if every type in the parameter pack is the same.
  using IsAllSameType =
      bool_constant<if_all<std::is_same<NthType<0>, Ts>...>::value>;
};

template <>
struct ParameterPack<> {
  // Checks if |Type| occurs in the parameter pack.
  template <typename Type>
  using HasType = bool_constant<false>;

  // Checks if the parameter pack only contains |Type|.
  template <typename Type>
  using OnlyHasType = bool_constant<true>;

  // Checks if |Type| occurs only once in the parameter pack.
  template <typename Type>
  using IsUniqueInPack = bool_constant<false>;

  // Returns the zero-based index of |Type| within |Pack...| or |pack_npos| if
  // it's not within the pack.
  template <typename Type>
  static constexpr size_t IndexInPack() {
    return pack_npos;
  }

  // Helper for extracting the Nth type from a parameter pack.
  template <size_t N>
  using NthType = void;

  // Checks if every type in the parameter pack is the same.
  using IsAllSameType =
      bool_constant<true>;
};

}  // namespace base

#endif  // BASE_PARAMETER_PACK_H_
