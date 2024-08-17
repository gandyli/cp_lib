#pragma once
#include "seed.hpp"

template <typename T>
struct Hash;

template <typename T>
u64 hash(const T& x) { return Hash<T>{}(x); }
template <typename T>
void hash_combine(u64& seed, const T& x) { seed ^= hash(x) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4); }

template <Integer T>
struct Hash<T> {
    u64 operator()(T x) const {
        if constexpr (sizeof(T) == 16) {
            u64 v{};
            hash_combine(v, u64(x));
            hash_combine(v, u64(x >> 64));
            return v;
        }
        u64 y = x;
        y += FIXED_RANDOM;
        y = (y ^ (y >> 30)) * 0xbf58476d1ce4e5b9;
        y = (y ^ (y >> 27)) * 0x94d049bb133111eb;
        return y ^ (y >> 31);
    }
};
template <typename T1, typename T2>
struct Hash<std::pair<T1, T2>> {
    u64 operator()(const std::pair<T1, T2>& p) const {
        u64 v{};
        hash_combine(v, p.first);
        hash_combine(v, p.second);
        return v;
    }
};
template <typename... Args>
struct Hash<std::tuple<Args...>> {
    u64 operator()(const std::tuple<Args...>& t) const {
        u64 v{};
        std::apply([&](auto&&... args) { (hash_combine(v, args), ...); }, t);
        return v;
    }
};
template <input_range R>
struct Hash<R> {
    u64 operator()(const R& r) const {
        u64 v{};
        foreach (x, r)
            hash_combine(v, x);
        return v;
    }
};