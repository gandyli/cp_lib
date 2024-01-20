#pragma once
#include "template.hpp"

struct RollingHash {
    static constexpr u64 P = (1ull << 61) - 1;
    vc<u64> power;
    static u64 add(u64 a, u64 b) {
        a += b;
        if (a >= P)
            a -= P;
        return a;
    }
    static constexpr u64 mul(u64 a, u64 b) {
        constexpr u64 mask30 = (1ull << 30) - 1,
                      mask31 = (1ull << 31) - 1,
                      mask61 = (1ull << 61) - 1;
        u64 au = a >> 31, ad = a & mask31;
        u64 bu = b >> 31, bd = b & mask31;
        u64 x = ad * bu + au * bd;
        u64 xu = x >> 30, xd = x & mask30;
        x = au * bu * 2 + xu + (xd << 31) + ad * bd;
        xu = x >> 61, xd = x & mask61;
        x = xu + xd;
        if (x >= mask61)
            x -= mask61;
        return x;
    }
    static u64 generateBase() {
        std::mt19937_64 rd(std::chrono::steady_clock::now().time_since_epoch().count());
        return std::uniform_int_distribution<u64>(1, P - 1)(rd);
    }
    const u64 base;
    RollingHash(u64 base = generateBase()): power{1}, base(base) {}
    template <typename T>
    vc<u64> build(T&& s) const {
        vc<u64> ans(s.size() + 1);
        for (std::size_t i = 0; i < s.size(); i++)
            ans[i + 1] = add(mul(ans[i], base), s[i]);
        return ans;
    }
    template <typename T>
    u64 hash(T&& s) const {
        u64 ans = 0;
        for (std::size_t i = 0; i < s.size(); i++)
            ans = add(mul(ans, base), s[i]);
        return ans;
    }
    void expand(int s) {
        int m = power.size();
        if (m <= s) {
            power.resize(s + 1);
            for (int i = m - 1; i < s; i++)
                power[i + 1] = mul(power[i], base);
        }
    }
    u64 query(const vc<u64>& h, int l, int r) {
        expand(r - l);
        return add(h[r], P - mul(h[l], power[r - l]));
    }
    u64 query(const vc<u64>& h, int r) {
        return query(h, 0, r);
    }
    u64 query(const vc<u64>& h) {
        return query(h, 0, h.size() - 1);
    }
    u64 combine(u64 h1, u64 h2, u64 s) {
        expand(s);
        return add(mul(h1, power[s]), h2);
    }
    int lcp(const vc<u64>& h0, int l0, int r0, const vc<u64>& h1, int l1, int r1) {
        int l = 0, r = std::min(r0 - l0, r1 - l1);
        while (l < r) {
            int m = (l + r + 1) >> 1;
            if (query(h0, l0, l0 + m) == query(h1, l1, l1 + m))
                l = m;
            else
                r = m - 1;
        }
        return l;
    }
};