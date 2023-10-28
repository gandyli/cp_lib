#pragma once
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
#define lowbit(x) ((x) & (-(x)))
#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)
#define LB(c, x) std::distance(std::begin(c), std::lower_bound(all(c), (x)))
#define UB(c, x) std::distance(std::begin(c), std::upper_bound(all(c), (x)))
#define UNIQUE(c) sort(c), (c).erase(std::unique(all(c)), std::end(c))
#define VEC(type, a, ...) auto a = vec<type>(__VA_ARGS__)
#define VECI(a, ...) auto a = veci(__VA_ARGS__)
#define FORWARD(x) std::forward<decltype(x)>(x)
#define eb emplace_back
// NOLINTEND

constexpr auto ceil(auto&& x, auto&& y) { return x > 0 ? (x + y - 1) / y : x / y; }
constexpr auto floor(auto&& x, auto&& y) { return x > 0 ? x / y : (x - y + 1) / y; }
constexpr auto divmod(auto x, auto y) {
    auto&& q = floor(x, y);
    return std::pair{q, x - q * y};
}
constexpr u64 ten(int t) { return t == 0 ? 1 : ten(t - 1) * 10; }
constexpr auto Max(const auto& x) { return x; }
constexpr auto Min(const auto& x) { return x; }
constexpr auto Max(const auto& x, const auto& y, const auto&... arg) { return x < y ? Max(y, arg...) : Max(x, arg...); }
constexpr auto Min(const auto& x, const auto& y, const auto&... arg) { return x < y ? Min(x, arg...) : Min(y, arg...); }
constexpr bool chkmax(auto& d, const auto&... x) {
    auto t = Max(x...);
    return t > d ? d = t, true : false;
}
constexpr bool chkmin(auto& d, const auto&... x) {
    auto t = Min(x...);
    return t < d ? d = t, true : false;
}
constexpr auto sum(input_range auto&& r) { return std::accumulate(begin(r), end(r), std::decay_t<decltype(*begin(r))>{}); }
constexpr auto sum(input_range auto&& r, auto init) { return std::accumulate(begin(r), end(r), init); }
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
template <typename T>
auto vec(usize n, auto&&... s) {
    if constexpr (!sizeof...(s))
        return Vec<T>(n);
    else
        return Vec(n, vec<T>(s...));
}
auto veci(usize n, auto&&... s) {
    if constexpr (sizeof...(s) == 1)
        return Vec(n, s...);
    else
        return Vec(n, veci(s...));
}
template <typename T>
vi argsort(Vec<T>& a) {
    vi p(len(a));
    std::iota(all(p), 0);
    sort(p, [&](int i, int j) { return std::pair{a[i], i} < std::pair{a[j], j}; });
    return p;
}
vi sshift(const str& s, char c = 'a') {
    vi a(len(s));
    _for (i, len(s))
        a[i] = s[i] - c;
    return a;
}
template <typename T>
T pop(Vec<T>& q) {
    T r = std::move(q.back());
    q.pop_back();
    return r;
}
template <typename T>
T pop(std::deque<T>& q) {
    T r = std::move(q.front());
    q.pop_front();
    return r;
}
template <typename T, typename S, typename C>
T pop(std::priority_queue<T, S, C>& q) {
    T r = q.top();
    q.pop();
    return r;
}
template <typename T>
constexpr T inf = std::numeric_limits<T>::max() * 0.49;
template <>
constexpr f64 inf<f64> = inf<i64>;
template <>
constexpr ld inf<ld> = inf<i64>;