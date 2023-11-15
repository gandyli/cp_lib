#pragma once
#include "modint/barrett_reduction.hpp"
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

template <typename T, u32 id = 0>
struct ArbitraryModIntBase {
    using mint = ArbitraryModIntBase;
    using int_type = T;

    static Barrett<T> bt;

    ArbitraryModIntBase() = default;
    ArbitraryModIntBase(Signed auto y) {
        y %= mod();
        x = y < 0 ? y + mod() : y;
    }
    ArbitraryModIntBase(Unsigned auto y) { x = y % mod(); }
    T val() const { return x; }
    static T mod() { return bt.mod(); }
    mint& operator++() {
        x++;
        if (x == mod())
            x = 0;
        return *this;
    }
    mint operator++(int) {
        mint r = *this;
        ++*this;
        return r;
    }
    mint& operator+=(const mint& p) {
        x += p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    mint& operator--() {
        if (x == 0)
            x = mod();
        x--;
        return *this;
    }
    mint operator--(int) {
        mint r = *this;
        --*this;
        return r;
    }
    mint& operator-=(const mint& p) {
        x += mod() - p.x;
        if (x >= mod())
            x -= mod();
        return *this;
    }
    mint& operator*=(const mint& p) {
        x = bt.mul(x, p.x);
        return *this;
    }
    mint inv() const { return from_raw(mod_inverse(x, mod())); }
    mint& operator/=(const mint& p) { return *this *= p.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return from_raw(!x ? 0 : mod() * 2 - x); }
    friend mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    auto operator<=>(const mint& rhs) const = default;

    static mint from_raw(T x) {
        mint r;
        r.x = x;
        return r;
    }
    T raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static T x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(x); }
#endif
    static void set_mod(T p) {
        ASSERT(0 < p && p <= std::numeric_limits<T>::max() / 4);
        bt = p;
    }

private:
    T x{};
};
template <typename T, u32 id>
Barrett<T> ArbitraryModIntBase<T, id>::bt;
using dmint32 = ArbitraryModIntBase<u32, 0>;
using dmint64 = ArbitraryModIntBase<u64, 0>;