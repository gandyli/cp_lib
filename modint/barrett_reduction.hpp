#pragma once
#include "template.hpp"

template <typename T>
class Barrett;

template <>
class Barrett<u32> {
    constexpr auto get(u64 n) const {
        u64 x = u128(n) * im >> 64;
        u64 y = x * m;
        return std::pair(x, y);
    }

public:
    u32 m{};
    u64 im{};
    constexpr Barrett(u32 m = 1): m(m), im(u64(-1) / m + 1) {}
    constexpr u32 mod() const { return m; }
    constexpr u64 div(u64 n) const {
        auto [x, y] = get(n);
        return x - (n < y);
    }
    constexpr u32 mod(u64 n) const {
        auto [x, y] = get(n);
        return n - y + (n < y ? m : 0);
    }
    constexpr std::pair<u64, u32> divmod(u64 n) const {
        auto [x, y] = get(n);
        if (n < y)
            return {x - 1, n - y + m};
        return {x, n - y};
    }
    constexpr u32 mul(u32 x, u32 y) const { return mod(u64(x) * y); }
};
template <>
class Barrett<u64> {
public:
    u64 m{}, imh{}, iml{};
    constexpr Barrett(u64 m = 1): m(m) {
        u128 im = u128(-1) / m;
        if (im * m + m == 0)
            im++;
        imh = im >> 64;
        iml = im;
    }
    constexpr u64 mod() const { return m; }
    constexpr u64 mod(u128 x) const {
        u128 z = u128(u64(x)) * iml;
        z = u128(u64(x)) * imh + (x >> 64) * iml + (z >> 64);
        z = (x >> 64) * imh + (z >> 64);
        x -= z * m;
        return x < m ? x : x - m;
    }
    constexpr u64 mul(u64 x, u64 y) const { return mod(u128(x) * y); }
};