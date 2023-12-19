#pragma once
#include <bits/stdc++.h>

#define Vec std::vector
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = unsigned __int128;
using usize = std::size_t;
using i8 = signed char;
using i16 = short;
using i64 = long long;
using i128 = __int128;
using ld = long double;
using vi = Vec<int>;
using vvi = Vec<vi>;
using vvvi = Vec<vvi>;
using pi = std::pair<int, int>;
template <typename T>
struct make_unsigned: public std::make_unsigned<T> {};
template <>
struct make_unsigned<i128> {
    using type = u128;
};

namespace impl {
    template <typename T, typename = void>
    struct has_tuple_element: std::false_type {};
    template <typename T>
    struct has_tuple_element<T, decltype(void(typename std::tuple_element<0, typename std::decay<T>::type>::type{}))>: std::true_type {};
} // namespace impl

template <typename T>
struct tupleLike: std::integral_constant<bool, impl::has_tuple_element<T>::value && !std::is_array<T>::value> {
};
template <typename T>
struct Signed: std::integral_constant<bool, (std::is_signed<T>::value && std::is_integral<T>::value) || std::is_same<T, i128>::value> {
};
template <typename T>
struct Unsigned: std::integral_constant<bool, (std::is_unsigned<T>::value && std::is_integral<T>::value) || std::is_same<T, u128>::value> {
};