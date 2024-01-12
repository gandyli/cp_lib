#pragma once
#include "utility/make_double_width.hpp"

template <typename T>
struct BarrettReduction {
    using int_type = T;
    using int_double_t = make_double_width_t<T>;
    constexpr explicit BarrettReduction(T m = 1): m(m), im(int_double_t(-1) / m + 1) {}
    constexpr T mod() const { return m; }
    constexpr int_double_t div(int_double_t n) const {
        auto [x, y] = get(n);
        return x - (n < y);
    }
    constexpr T mod(int_double_t n) const {
        auto [x, y] = get(n);
        return n - y + (n < y ? m : 0);
    }
    constexpr std::pair<int_double_t, T> divmod(int_double_t n) const {
        auto [x, y] = get(n);
        if (n < y)
            return {x - 1, n - y + m};
        return {x, n - y};
    }
    constexpr T mul(T x, T y) const { return mod(int_double_t(x) * y); }

private:
    T m{};
    int_double_t im{};
    constexpr auto get(int_double_t n) const {
        int_double_t x;
        if constexpr (sizeof(T) == 8) {
            u64 iml = im, imh = im >> 64;
            x = (n >> 64) * imh + ((u128(u64(n)) * imh + (n >> 64) * iml + (u128(u64(n)) * iml >> 64)) >> 64);
        }
        else
            x = make_double_width_t<int_double_t>(n) * im >> (sizeof(T) * 16);
        return std::pair{x, x * m};
    }
};