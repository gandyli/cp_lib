#pragma once
#include "modint/barrett_reduction.hpp"
#include "math/mod_inverse.hpp"
#include "math/power.hpp"

template <int id>
struct ArbitraryModIntBase {
    using mint = ArbitraryModIntBase;

    ArbitraryModIntBase() = default;
    ArbitraryModIntBase(Signed auto y) {
        y %= mod();
        x = y < 0 ? y + mod() : y;
    }
    ArbitraryModIntBase(Unsigned auto y) { x = y % mod(); }
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
        x = rem(u64(x) * p.x);
        return *this;
    }
    mint& operator/=(const mint& p) { return *this *= p.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return mint(-x); }
    friend mint operator+(mint lhs, const mint& rhs) { return lhs += rhs; }
    friend mint operator-(mint lhs, const mint& rhs) { return lhs -= rhs; }
    friend mint operator*(mint lhs, const mint& rhs) { return lhs *= rhs; }
    friend mint operator/(mint lhs, const mint& rhs) { return lhs /= rhs; }
    auto operator<=>(const mint& rhs) const = default;
    mint inv() const { return from_raw(mod_inverse<u32>(x, mod())); }
    int val() const { return x; }
    static mint from_raw(int x) {
        mint r;
        r.x = x;
        return r;
    }
    int raw() const { return x; }
#ifdef FASTIO
    void read(IO& io) {
        static int x;
        io.read(x);
        *this = x;
    }
    void write(IO& io) const { io.write(x); }
#endif
    static int& mod() {
        static int mod = 0;
        return mod;
    }
    static void set_mod(int p) {
        ASSERT(0 < p && p <= (1 << 30) - 1);
        mod() = p;
        barrett() = Barrett(p);
    }

private:
    int x{};

    u32 rem(u64 p) { return barrett().rem(p); }
    static Barrett& barrett() {
        static Barrett b;
        return b;
    }
};

using ArbitraryModInt = ArbitraryModIntBase<-1>;