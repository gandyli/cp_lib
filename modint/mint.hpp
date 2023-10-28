#pragma once
#include "template.hpp"

template <typename T, typename U>
constexpr T power(T a, U b) {
    T ans = 1;
    for (; b; b >>= 1, a *= a)
        if (b & 1)
            ans *= a;
    return ans;
}

template <u32 P>
struct mint {
    static constexpr u32 mod = P;
    static constexpr u32 r = []() {
        u32 r = P;
        _for (4)
            r *= 2 - P * r;
        return -r;
    }();
    static constexpr u32 n2 = -u64(P) % P;
    static_assert(r * P == -1);
    static_assert(P < (1 << 30));
    static_assert(P % 2 == 1);

    u32 a;

    constexpr mint(): a(0) {}
    constexpr mint(i64 b): a(reduce(u64(b % P + P) * n2)) {}

    static constexpr u32 reduce(u64 b) { return (b + u64(u32(b) * r) * P) >> 32; }

    constexpr mint& operator+=(mint b) {
        a += b.a - 2 * P;
        if (int(a) < 0)
            a += 2 * P;
        return *this;
    }
    constexpr mint& operator-=(mint b) {
        a -= b.a;
        if (int(a) < 0)
            a += 2 * P;
        return *this;
    }
    constexpr mint& operator*=(mint b) {
        a = reduce(u64(a) * b.a);
        return *this;
    }
    constexpr mint& operator/=(mint b) {
        return *this *= b.inv();
    }
    constexpr mint operator+(mint b) const { return mint(*this) += b; }
    constexpr mint operator-(mint b) const { return mint(*this) -= b; }
    constexpr mint operator*(mint b) const { return mint(*this) *= b; }
    constexpr mint operator/(mint b) const { return mint(*this) /= b; }
    constexpr bool operator==(const mint& b) const {
        return (a >= P ? a - P : a) == (b.a >= P ? b.a - P : b.a);
    }
    constexpr bool operator!=(const mint& b) const {
        return (a >= P ? a - P : a) != (b.a >= P ? b.a - P : b.a);
    }
    constexpr mint operator-() const { return mint() - mint(*this); }
    constexpr mint pow(u64 n) const {
        mint r(1), a(*this);
        while (n) {
            if (n & 1)
                r *= a;
            a *= a;
            n >>= 1;
        }
        return r;
    }
    constexpr mint inv() const { return pow(P - 2); }
    constexpr u32 get() const {
        u32 r = reduce(a);
        return r >= P ? r - P : r;
    }
};

using Z = mint<1'000'000'007>;

template <typename u32, typename u64>
struct lazymint {
    using mint = lazymint;
    using int_type = u32;
    using i32 = std::make_signed_t<u32>;

    inline static u32 P, r, n2;
    static void set_mod(u32 m) {
        P = m;
        // assert(P < (1ull << ((sizeof(u32) << 3) - 2)));
        // assert(P % 2 == 1);
        n2 = -u64(P) % P;
        r = P;
        _for (5)
            r *= 2 - P * r;
        r = -r;
        // assert(r * P == -1);
    }

    static u32 reduce(u64 b) { return (b + u64(u32(b) * r) * P) >> (sizeof(u32) << 3); }
    u32 a;

    lazymint(): a(0) {}
    lazymint(::u64 b): a(reduce(u64(b) * n2)) {}

    mint& operator+=(mint b) {
        a += b.a - 2 * P;
        if (i32(a) < 0)
            a += 2 * P;
        return *this;
    }
    mint& operator-=(mint b) {
        a -= b.a;
        if (i32(a) < 0)
            a += 2 * P;
        return *this;
    }
    mint& operator*=(mint b) {
        a = reduce(u64(a) * b.a);
        return *this;
    }
    mint& operator/=(mint b) {
        return *this *= b.inv();
    }
    mint operator+(mint b) const { return mint(*this) += b; }
    mint operator-(mint b) const { return mint(*this) -= b; }
    mint operator*(mint b) const { return mint(*this) *= b; }
    mint operator/(mint b) const { return mint(*this) /= b; }
    bool operator==(const mint& b) const {
        return (a >= P ? a - P : a) == (b.a >= P ? b.a - P : b.a);
    }
    bool operator!=(const mint& b) const {
        return (a >= P ? a - P : a) != (b.a >= P ? b.a - P : b.a);
    }
    mint operator-() const { return mint() - mint(*this); }
    mint pow(u64 n) const {
        mint r(1), a(*this);
        while (n) {
            if (n & 1)
                r *= a;
            a *= a;
            n >>= 1;
        }
        return r;
    }
    mint inv() const { return pow(P - 2); }
    u32 get() const {
        u32 r = reduce(a);
        return r >= P ? r - P : r;
    }
};

using m32 = lazymint<u32, u64>;
using m64 = lazymint<u64, u128>;