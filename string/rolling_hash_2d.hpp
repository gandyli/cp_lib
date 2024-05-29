#pragma once
#include "random/base.hpp"
#include "modint/mint61.hpp"

template <typename MINT = MMInt61>
struct Rollinghash_2D {
    using mint = MINT;
    static inline vc<mint> p1{1}, p2{1};
    static u64 generateBase() {
        if constexpr (Modint<mint>)
            return rnd(2, mint::mod());
        else
            return 2 * rnd(2, std::numeric_limits<mint>::max() / 2) + 1;
    }
    static inline const mint base1 = generateBase(), base2 = generateBase();

    vvc<mint> h;
    void build(auto&& s) {
        int n = len(s), m = len(s[0]);
        h = vec<mint>(n + 1, m + 1);
        _for (i, n) {
            _for (j, m)
                h[i + 1][j + 1] = h[i + 1][j] * base2 + (s[i][j] + 1);
            _for (j, m + 1)
                h[i + 1][j] += h[i][j] * base1;
        }
        expand(p1, base1, n);
        expand(p2, base2, m);
    }
    void expand(auto& p, mint b, u32 s) {
        if (u32 m = len(p); m <= s) {
            p.resize(s + 1);
            _for (i, m - 1, s)
                p[i + 1] = p[i] * b;
        }
    }
    Rollinghash_2D(random_access_range auto&& s) { build(s); }
    mint query(int xl, int xr, int yl, int yr) const {
        mint a = h[xr][yr] - h[xr][yl] * p2[yr - yl];
        mint b = h[xl][yr] - h[xl][yl] * p2[yr - yl];
        return a - b * p1[xr - xl];
    }
};