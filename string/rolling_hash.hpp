#pragma once
#include "random/base.hpp"
#include "modint/mint61.hpp"
#include "math/binary_search.hpp"

template <typename MINT = MMInt61>
class Rollinghash {
    using mint = MINT;
    static vc<mint> p;
    static u64 generateBase() { return rnd(1, mint::mod()); }
    static const mint base;
    void build(auto&& s) {
        build(len(s), [&](int i) { return s[i]; });
    }
    void build(int n, std::invocable<int> auto&& f) {
        h.resize(n + 1);
        _for (i, n)
            h[i + 1] = h[i] * base + f(i);
    }
    static void expand(u32 s) {
        if (u32 m = len(p); m <= s) {
            p.resize(s + 1);
            _for (i, m - 1, s)
                p[i + 1] = p[i] * base;
        }
    }
    vc<mint> h;

public:
    Rollinghash(random_access_range auto&& s) { build(s); }
    Rollinghash(int n, std::invocable<int> auto&& f) { build(n, f); }
    static mint hash(auto&& s) {
        mint ans = 0;
        _for (i, len(s))
            ans = ans * base + s[i];
        return ans;
    }
    mint query(u32 l, u32 r) const {
        expand(r - l);
        return h[r] - h[l] * p[r - l];
    }
    mint query(u32 r) const { return h[r]; }
    mint query() const { return h.back(); }
    static mint combine(mint h1, mint h2, u32 s) {
        expand(s);
        return h1 * p[s] + h2;
    }
    static u32 lcp(const Rollinghash& h0, u32 l0, u32 r0, const Rollinghash& h1, u32 l1, u32 r1) {
        auto check = [&](auto mi) { return h0.query(l0, l0 + mi) == h1.query(l1, l1 + mi); };
        return bsearch<int>(check, 0, min(r0 - l0, r1 - l1) + 1);
    }
};
template <typename MINT>
const MINT Rollinghash<MINT>::base = Rollinghash<MINT>::generateBase();
template <typename MINT>
vc<MINT> Rollinghash<MINT>::p{1};