#pragma once
#include "random/base.hpp"
#include "modint/mint61.hpp"

class Rollinghash {
    using mint = MMInt61;
    static Vec<mint> p;
    static u64 generateBase() { return rnd(1, mint::mod()); }
    static const mint base;
    void init(auto&& s) {
        h.resize(len(s) + 1);
        _for (i, len(s))
            h[i + 1] = h[i] * base + s[i];
    }
    static void expand(u32 s) {
        if (u32 m = len(p); m <= s) {
            p.resize(s + 1);
            _for (i, m - 1, s)
                p[i + 1] = p[i] * base;
        }
    }
    Vec<mint> h;

public:
    template <typename T>
    Rollinghash(const Vec<T>& s) { init(s); }
    Rollinghash(const str& s) { init(s); }
    static mint hash(auto&& s) {
        mint ans = 0;
        _for (i, len(s))
            ans = ans * base + s[i];
        return ans;
    }
    auto query(u32 l, u32 r) const {
        expand(r - l);
        return h[r] - h[l] * p[r - l];
    }
    auto query(u32 r) const { return h[r]; }
    auto query() const { return h.back(); }
    static auto combine(mint h1, mint h2, u32 s) {
        expand(s);
        return h1 * p[s] + h2;
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
const Rollinghash::mint Rollinghash::base = Rollinghash::generateBase();
Vec<Rollinghash::mint> Rollinghash::p{1ULL};