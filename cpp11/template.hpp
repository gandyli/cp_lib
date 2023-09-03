#pragma once
#include "types.hpp"

#include <bits/stdc++.h>

// NOLINTBEGIN
#define FOR1(a) for (typename std::decay<decltype(a)>::type _ = 0; _ < (a); _++)
#define FOR2(i, a) for (typename std::decay<decltype(a)>::type i = 0; i < (a); i++)
#define FOR3(i, a, b) for (auto i = a; i < (b); i++)
#define FOR4(i, a, b, c) for (auto i = a; i < (b); i += (c))
#define FOR1_R(a) for (auto i = (a)-1; i >= 0; i--)
#define FOR2_R(i, a) for (auto i = (a)-1; i >= 0; i--)
#define FOR3_R(i, a, b) for (auto i = (b)-1; i >= (a); i--)
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define _for(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define _for_r(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define foreach(x, a) for (auto&& x: a)
#define loop while (true)
// NOLINTEND

template <typename T, typename U>
constexpr auto ceil(T&& x, U&& y) -> decltype(x > 0 ? (x + y - 1) / y : x / y) { return x > 0 ? (x + y - 1) / y : x / y; }
template <typename T, typename U>
constexpr auto floor(T&& x, U&& y) -> decltype(x > 0 ? x / y : (x - y + 1) / y) { return x > 0 ? x / y : (x - y + 1) / y; }
template <typename T, typename U>
auto divmod(T&& x, U&& y) -> decltype(std::make_pair(floor(x, y), x - floor(x, y) * y)) {
    auto&& q = floor(x, y);
    return std::make_pair(q, x - q * y);
}
constexpr u64 ten(int t) { return t == 0 ? 1 : ten(t - 1) * 10; }
template <typename T, typename P>
auto max(const T& x, const P& y) -> decltype(x < y ? y : x) { return x < y ? y : x; }
template <typename T, typename P>
auto min(const T& x, const P& y) -> decltype(x < y ? x : y) { return x < y ? x : y; }
template <typename T, typename... Args>
auto max(const T& x, const Args&... args) -> decltype(max(x, max(args...))) { return max(x, max(args...)); }
template <typename T, typename... Args>
auto min(const T& x, const Args&... args) -> decltype(min(x, min(args...))) { return min(x, min(args...)); }
template <typename T>
T max(const T& x) { return x; }
template <typename T>
T min(const T& x) { return x; }
template <typename T, typename... Args>
void chkmax(T& d, const Args&... x) { d = max(d, x...); }
template <typename T, typename... Args>
void chkmin(T& d, const Args&... x) { d = min(d, x...); }
template <typename Iterator, typename = void>
struct is_forward_iterator: std::false_type {};
template <typename Iterator>
struct is_forward_iterator<Iterator, decltype(++std::declval<Iterator&>(), void(*std::declval<Iterator>()))>: std::true_type {};
template <typename T>
struct forward_range {
    template <typename U>
    static auto test(U&& u) -> decltype(std::begin(std::forward<U>(u)), std::end(std::forward<U>(u)), std::true_type());
    static auto test(...) -> std::false_type;
    static constexpr bool value = decltype(test(std::declval<T>()))::value;
};
template <typename I>
using decayed_iterator_value_t = typename std::decay<decltype(*std::declval<I>())>::type;

template <typename R>
typename std::enable_if<forward_range<R>::value, decayed_iterator_value_t<decltype(std::begin(std::declval<R>()))>>::type
sum(R&& r) {
    using I = decltype(std::begin(r));
    return std::accumulate(std::begin(r), std::end(r), decayed_iterator_value_t<I>{});
}

template <typename R, typename T>
typename std::enable_if<forward_range<R>::value, T>::type
sum(R&& r, T init) {
    return std::accumulate(std::begin(r), std::end(r), init);
}

template <typename T>
constexpr int len(const T& x) { return x.size(); }
template <typename T, std::size_t N>
constexpr int len(const T (&)[N]) { return N; }

template <typename T>
Vec<T> psum(const Vec<T>& a) {
    int n = len(a);
    Vec<T> b(len(a) + 1);
    _for(i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}
template <typename T, typename U>
Vec<T> psum(U&& a) {
    int n = len(a);
    Vec<T> b(len(a) + 1);
    _for(i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}

template <typename T>
Vec<T> vec(usize n) { return Vec<T>(n); }
template <typename T, typename... Args>
auto vec(usize n, Args&&... s) -> decltype(Vec<decltype(vec<T>(s...))>(n, vec<T>(s...))) {
    return Vec<decltype(vec<T>(s...))>(n, vec<T>(s...));
}
template <typename T>
Vec<typename std::decay<T>::type> veci(usize n, T&& x) {
    return Vec<typename std::decay<T>::type>(n, x);
}
template <typename... Args>
auto veci(usize n, Args&&... s) -> Vec<decltype(veci(s...))> {
    return Vec<decltype(veci(s...))>(n, veci(s...));
}
// template <typename... Args>
// auto veci(usize n, Args&&... s) {
//     if (sizeof...(s) == 1)
//         return Vec(n, s...);
//     else
//         return Vec(n, veci(s...));
// }

#define lc (o << 1)
#define rc (o << 1 | 1)
#define lowbit(x) ((x) & (-(x)))
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)
#define LB(c, x) std::distance(std::begin(c), std::lower_bound(all(c), (x)))
#define UB(c, x) std::distance(std::begin(c), std::upper_bound(all(c), (x)))
#define UNIQUE(c) std::sort(all(c)), (c).erase(std::unique(all(c)), std::end(c))
#define VEC(type, a, ...) auto a = vec<type>(__VA_ARGS__)
#define VECI(a, ...) auto a = veci(__VA_ARGS__)
#define dR(type, ...) \
    type __VA_ARGS__; \
    io.read(__VA_ARGS__)
#define dRV(type, a, ...)      \
    VEC(type, a, __VA_ARGS__); \
    io.read(a)

template <typename T>
vi argsort(Vec<T>& a) {
    vi p(len(a));
    std::iota(all(p), 0);
    sort(p, [&](int i, int j) { return std::make_pair(a[i], i) < std::make_pair(a[j], j); });
    return p;
}
