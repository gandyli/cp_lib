#pragma once
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

struct MInt61 {
    using mint = MInt61;
    using int_type = u64;

    constexpr MInt61() = default;
    constexpr MInt61(Signed auto y) {
        using S = std::make_signed_t<int_type>;
        S v = y % S(mod());
        x = v < 0 ? v + mod() : v;
    }
    constexpr MInt61(Unsigned auto y) { x = y % mod(); }
    constexpr int_type val() const { return x; }
    static constexpr int_type mod() { return (1ULL << 61) - 1; }
    constexpr mint& operator++() {
        x++;
        if (x == mod())
            x = 0;
        return *this;
    }
    constexpr mint operator++(int) {
        mint r = *this;
        ++*this;
        return r;
    }
    constexpr mint& operator+=(const mint& p) {
        x += p.x;
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
        mint r = *this;
        --*this;
        return r;
    }
    constexpr mint& operator-=(const mint& p) {
        x += mod() - p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint& operator*=(const mint& p) {
        u128 t = u128(x) * p.x;
        x = u64(t >> 61) + u64(t & mod());
        if (x >= mod())
            x -= mod();
        return *this;
    }
    constexpr mint inv() const { return from_raw(mod_inverse(x, mod())); }
    constexpr mint& operator/=(const mint& p) { return *this *= p.inv(); }
    constexpr mint operator+() const { return *this; }
    constexpr mint operator-() const { return from_raw(!x ? 0 : mod() - x); }
    friend constexpr mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend constexpr mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend constexpr mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend constexpr mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    friend constexpr auto operator<=>(const mint&, const mint&) = default;

    static constexpr mint from_raw(int_type x) {
        mint r;
        r.x = x;
        return r;
    }
    constexpr int_type raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static int_type x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(val()); }
#endif

private:
    int_type x{};
};