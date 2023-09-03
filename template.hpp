#pragma once
#include <bits/stdc++.h>
#include "types.hpp"
using namespace std::ranges;

// NOLINTBEGIN
#define FOR1(a) for (std::decay_t<decltype(a)> _ = 0; _ < (a); _++)
#define FOR2(i, a) for (std::decay_t<decltype(a)> i = 0; i < (a); i++)
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
[[maybe_unused]] struct {
    constexpr auto operator->*(auto&& f) const { return f(); }
} blk;
#define BLK blk->*[&]
// NOLINTEND

template <typename T, typename U>
constexpr auto ceil(T&& x, U&& y) { return x > 0 ? (x + y - 1) / y : x / y; }
template <typename T, typename U>
constexpr auto floor(T&& x, U&& y) { return x > 0 ? x / y : (x - y + 1) / y; }
template <typename T, typename U>
constexpr auto divmod(T&& x, U&& y) {
    auto&& q = floor(x, y);
    return std::pair{q, x - q * y};
}
constexpr u64 ten(int t) { return t == 0 ? 1 : ten(t - 1) * 10; }
template <typename T, typename... Args>
constexpr void chkmax(T& d, const Args&... x) { d = max({d, x...}); }
template <typename T, typename... Args>
constexpr void chkmin(T& d, const Args&... x) { d = min({d, x...}); }

template <forward_range R>
constexpr auto sum(R&& r) { return std::accumulate(begin(r), end(r), std::decay_t<decltype(*begin(r))>{}); }
template <forward_range R, typename T>
constexpr auto sum(R&& r, T init) { return std::accumulate(begin(r), end(r), init); }
constexpr int len(auto&& x) { return std::size(x); }

template <typename T>
auto psum(const Vec<T>& a) {
    int n = len(a);
    Vec<T> b(len(a) + 1);
    _for (i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}
template <typename T>
auto psum(auto&& a) {
    int n = len(a);
    Vec<T> b(len(a) + 1);
    _for (i, n)
        b[i + 1] = b[i] + a[i];
    return b;
}

template <typename T, typename... Args>
auto vec(usize n, Args&&... s) {
    if constexpr (!sizeof...(s))
        return Vec<T>(n);
    else
        return Vec(n, vec<T>(s...));
}
template <typename... Args>
auto veci(usize n, Args&&... s) {
    if constexpr (sizeof...(s) == 1)
        return Vec(n, s...);
    else
        return Vec(n, veci(s...));
}

#define lc (o << 1)
#define rc (o << 1 | 1)
#define lowbit(x) ((x) & (-(x)))
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)
#define LB(c, x) std::distance(std::begin(c), std::lower_bound(all(c), (x)))
#define UB(c, x) std::distance(std::begin(c), std::upper_bound(all(c), (x)))
#define UNIQUE(c) sort(c), (c).erase(std::unique(all(c)), std::end(c))
#define VEC(type, a, ...) auto a = vec<type>(__VA_ARGS__)
#define VECI(a, ...) auto a = veci(__VA_ARGS__)
#define eb emplace_back

template <typename T>
vi argsort(Vec<T>& a) {
    vi p(len(a));
    std::iota(all(p), 0);
    sort(p, [&](int i, int j) { return std::pair{a[i], i} < std::pair{a[j], j}; });
    return p;
}
