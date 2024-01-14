#pragma once
#include "random/base.hpp"
#include "modint/mint61.hpp"

template <typename T>
u64 hash_vector(const Vec<T>& a) {
    using mint = MMInt61;
    static Vec<mint> p;
    int n = len(a);
    p.reserve(n + 1);
    while (len(p) <= n)
        p.eb(rnd(0, mint::mod()));
    mint H = 0;
    _for (i, n)
        H += p[i] * a[i];
    H += p[n];
    return H.val();
}
template <typename>
struct hash {};
template <typename T>
void hash_combine(u64& seed, const T& v) { seed ^= hash<T>{}(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4); }
template <Integer T>
struct hash<T> {
    u64 operator()(const T& x) const {
        if constexpr (sizeof(T) == 16) {
            u64 v = 0;
            hash_combine(v, u64(x));
            hash_combine(v, u64(x >> 64));
            return v;
        }
        return (x + FIXED_RANDOM) * 11995408973635179863ULL;
    }
};
template <typename T1, typename T2>
struct hash<std::pair<T1, T2>> {
    u64 operator()(const std::pair<T1, T2>& p) const {
        u64 v = 0;
        hash_combine(v, p.first);
        hash_combine(v, p.second);
        return v;
    }
};
template <typename... Args>
struct hash<std::tuple<Args...>> {
    u64 operator()(const std::tuple<Args...>& t) const {
        u64 v = 0;
        std::apply([&](auto&&... args) { (hash_combine(v, args), ...); }, t);
        return v;
    }
};
template <input_range R>
struct hash<R> {
    u64 operator()(const R& r) const {
        u64 v = 0;
        foreach (x, r)
            hash_combine(v, x);
        return v;
    }
};
