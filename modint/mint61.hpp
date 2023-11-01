#pragma once
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

struct MMInt61 {
    using mint = MMInt61;
    using int_type = u64;
    constexpr MMInt61() = default;
    constexpr MMInt61(Signed auto y) {
        y %= mod();
        x = y < 0 ? y + mod() : y;
    }
    constexpr MMInt61(Unsigned auto y) { x = y % mod(); }
    constexpr int_type val() const { return x; }
    static constexpr int_type mod() { return (1ULL << 61) - 1; }
    constexpr mint& operator++() {
        x++;
        if (x == mod())
            x = 0;
        return *this;
    }
    constexpr mint operator++(int) {
        mint ret = *this;
        ++*this;
        return ret;
    }
    constexpr mint& operator+=(const mint& rhs) {
        x += rhs.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint& operator--() {
        if (x == 0)
            x = mod();
        x--;
        return *this;
    }
    constexpr mint operator--(int) {
        mint ret = *this;
        --*this;
        return ret;
    }
    constexpr mint& operator-=(const mint& rhs) {
        x += mod() - rhs.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint& operator*=(const mint& rhs) {
        u128 t = u128(x) * rhs.x;
        x = u64(t >> 61) + u64(t & mod());
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint inv() const { return from_raw(mod_inverse(x, mod())); }
    constexpr mint& operator/=(const mint& rhs) { return *this *= rhs.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(x == 0 ? 0 : mod() - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    constexpr bool operator<=>(const mint& rhs) const = default;
    static constexpr mint from_raw(u64 x) {
        mint ret;
        ret.x = x;
        return ret;
    }
    constexpr u64 raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static u64 x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(val()); }
#endif

private:
    u64 x{};
};