#pragma once

#include <bits/stdc++.h>

#define Vec std::vector
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = unsigned __int128;
using usize = std::uintptr_t;
using isize = std::intptr_t;
using i8 = signed char;
using i16 = short;
using i32 = int;
using i64 = long long;
using i128 = __int128;
using ld = long double;
using vi = Vec<int>;
using vvi = Vec<vi>;
using vvvi = Vec<vvi>;
using pi = std::pair<int, int>;
using str = std::string;
template <typename T>
struct make_unsigned: public std::make_unsigned<T> {};
template <>
struct make_unsigned<i128> {
    using type = u128;
};
template <typename T>
using make_unsigned_t = make_unsigned<T>::type;
template <typename T>
concept tupleLike = requires { typename std::tuple_element_t<0, std::decay_t<T>>; } && !requires (T t) { t[0]; };
template <typename T>
concept Signed = std::signed_integral<T> || std::is_same_v<T, i128>;
template <typename T>
concept Unsigned = std::unsigned_integral<T> || std::is_same_v<T, u128>;
