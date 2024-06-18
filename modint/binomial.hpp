#pragma once
#include "template.hpp"

template <typename mint, bool EXTEND = true>
struct Comb {
    vc<mint> f, g, h;

    Comb() requires EXTEND
        : f{1}, g{1}, h{1} {}
    Comb(int n): f{1}, g{1}, h{1} { extend(n + 1); }
    void extend(int m = -1) {
        int n = len(f);
        if (m == -1)
            m = n * 2;
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
    mint finv(int x) {
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
        return fac(n) * finv(m) * finv(n - m);
    }
    mint P(int n, int m) {
        if (n < m || m < 0)
            return 0;
        return fac(n) * finv(n - m);
    }
    template <Integer T>
    mint multinomial(const vc<T>& a) {
        int n = 0;
        mint r = 1;
        foreach (x, a) {
            n += x;
            r *= finv(x);
        }
        r *= fac(n);
        return r;
    }
    mint operator()(int n, int m) { return C(n, m); }
    template <Integer T>
    mint operator()(const vc<T>& a) { return multinomial(a); }
};