#pragma once
#include "seed.hpp"

struct hash {
    template <typename T>
    static void hash_combine(u64& seed, const T& v) { seed ^= hash{}(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4); }
    template <Integer T>
    u64 operator()(const T& x) const {
        if constexpr (sizeof(T) == 16) {
            u64 v{};
            hash_combine(v, u64(x));
            hash_combine(v, u64(x >> 64));
            return v;
        }
        return (x + FIXED_RANDOM) * 11995408973635179863ULL;
    }
    template <typename T1, typename T2>
    u64 operator()(const std::pair<T1, T2>& p) const {
        u64 v{};
        hash_combine(v, p.first);
        hash_combine(v, p.second);
        return v;
    }
    template <typename... Args>
    u64 operator()(const std::tuple<Args...>& t) const {
        u64 v{};
        std::apply([&](auto&&... args) { (hash_combine(v, args), ...); }, t);
        return v;
    }
    template <input_range R>
    u64 operator()(const R& r) const {
        u64 v{};
        foreach (x, r)
            hash_combine(v, x);
        return v;
    }
};
