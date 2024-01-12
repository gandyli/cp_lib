#pragma once
#include "template.hpp"

template <typename T>
struct make_double_width {};
template <>
struct make_double_width<u8> {
    using type = u16;
};
template <>
struct make_double_width<u16> {
    using type = u32;
};
template <>
struct make_double_width<u32> {
    using type = u64;
};
template <>
struct make_double_width<u64> {
    using type = u128;
};
template <typename T>
using make_double_width_t = make_double_width<T>::type;