#pragma once
#include "../template.hpp"

class Rollinghash {
    static constexpr u64 P = (1ULL << 61) - 1;
    static Vec<u64> p;
    static constexpr u64 add(u64 a, u64 b) {
        a += b;
        if (a >= P)
            a -= P;
        return a;
    }
    static constexpr u64 mul(u64 a, u64 b) { return u128(a) * b % P; }
    static u64 generateBase() {
        std::mt19937_64 rd(std::chrono::steady_clock::now().time_since_epoch().count());
        return std::uniform_int_distribution<u64>(1, P - 1)(rd);
    }
    static const u64 base;
    void init(auto&& s) {
        h.resize(s.size() + 1);
        _for (i, len(s))
            h[i + 1] = add(mul(h[i], base), s[i]);
    }
    static void expand(u32 s) {
        if (u32 m = p.size(); m <= s) {
            p.resize(s + 1);
            _for (i, m - 1, s)
                p[i + 1] = mul(p[i], base);
        }
    }
    Vec<u64> h;

public:
    template <typename T>
    Rollinghash(const Vec<T>& s) { init(s); }
    Rollinghash(const str& s) { init(s); }
    static u64 hash(auto&& s) {
        u64 ans = 0;
        _for (i, len(s))
            ans = add(mul(ans, base), s[i]);
        return ans;
    }
    [[nodiscard]] auto query(u32 l, u32 r) const {
        expand(r - l);
        return add(h[r], P - mul(h[l], p[r - l]));
    }
    [[nodiscard]] auto query(u32 r) const { return h[r]; }
    [[nodiscard]] auto query() const { return h.back(); }
    static auto combine(u64 h1, u64 h2, u64 s) {
        expand(s);
        return add(mul(h1, p[s]), h2);
    }
    static u32 lcp(const Rollinghash& h0, u32 l0, u32 r0, const Rollinghash& h1, u32 l1, u32 r1) {
        u32 l = 0, r = min(r0 - l0, r1 - l1);
        while (l < r) {
            u32 m = (l + r + 1) >> 1;
            if (h0.query(l0, l0 + m) == h1.query(l1, l1 + m))
                l = m;
            else
                r = m - 1;
        }
        return l;
    }
};
const u64 Rollinghash::base = Rollinghash::generateBase();
Vec<u64> Rollinghash::p{1ULL};