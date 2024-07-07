#pragma once
#include "template.hpp"

#ifndef COMB_EXTEND
#define COMB_EXTEND true
#endif

template <typename mint, bool EXTEND = COMB_EXTEND>
struct Comb {
    vc<mint> f, g, h;

    Comb() = default;
    Comb(int n) { extend(n + 1); }
    void extend(int m = -1) {
        int n = len(f);
        if (n == 0)
            n = 1, f = g = h = {1};
        if (m == -1)
            m = n * 2;
        m++;
        chkmin(m, mint::mod());
        if (m <= n)
            return;
        f.resize(m);
        g.resize(m);
        h.resize(m);
        _for (i, n, m)
            f[i] = f[i - 1] * i;
        g[m - 1] = f[m - 1].inv();
        h[m - 1] = g[m - 1] * f[m - 2];
        _for_r (i, n, m - 1) {
            g[i] = g[i + 1] * (i + 1);
            h[i] = g[i] * f[i - 1];
        }
    }
    mint fac(int x) {
        if (x < 0)
            return {};
        if constexpr (EXTEND)
            while (x >= len(f))
                extend();
        return f[x];
    }
    mint fac_inv(int x) {
        if (x < 0)
            return {};
        if constexpr (EXTEND)
            while (x >= len(g))
                extend();
        return g[x];
    }
    mint inv(int x) {
        if (x < 0)
            return -inv(-x);
        if constexpr (EXTEND)
            while (x >= len(h))
                extend();
        return h[x];
    }
    mint C(int n, int m) {
        if (n < m || m < 0)
            return 0;
        return fac(n) * fac_inv(m) * fac_inv(n - m);
    }
    mint P(int n, int m) {
        if (n < m || m < 0)
            return 0;
        return fac(n) * fac_inv(n - m);
    }
    template <Integer T>
    mint multinomial(const vc<T>& a) {
        int n = 0;
        mint r = 1;
        foreach (x, a) {
            n += x;
            r *= fac_inv(x);
        }
        r *= fac(n);
        return r;
    }
    mint operator()(int n, int m) { return C(n, m); }
    template <Integer T>
    mint operator()(const vc<T>& a) { return multinomial(a); }
};
template <typename mint, bool EXTEND = COMB_EXTEND>
Comb<mint, EXTEND> comb;
template <typename mint>
mint fac(int x) { return comb<mint>.fac(x); }
template <typename mint>
mint fac_inv(int x) { return comb<mint>.fac_inv(x); }
template <typename mint>
mint inv(int x) { return comb<mint>.inv(x); }
template <typename mint>
mint C(int n, int m) { return comb<mint>.C(n, m); }
template <typename mint>
mint P(int n, int m) { return comb<mint>.P(n, m); }
template <typename mint, Integer T>
mint multinomial(const vc<T>& a) { return comb<mint>(a); }