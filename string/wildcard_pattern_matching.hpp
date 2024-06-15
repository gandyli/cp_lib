#pragma once
#include "poly/middle_product.hpp"
#include "random/base.hpp"

vcb wildcard_pattern_matching(const str& s, const str& t, char wildcard = '?') {
    using mint = MMInt998244353;
    int shift = rnd(0, mint::mod());
    int n = len(s), m = len(t);
    int mi = 1024;
    foreach (c, s)
        if (c != wildcard)
            chkmin(mi, c);
    foreach (c, t)
        if (c != wildcard)
            chkmin(mi, c);
    vc<mint> f1(n), g1(m), f2(n), g2(m), f3(n), g3(m);
    _for (i, n)
        f1[i] = s[i] == wildcard ? 0 : s[i] - mi + 1 + shift;
    _for (i, m)
        g1[i] = t[i] == wildcard ? 0 : t[i] - mi + 1 + shift;
    _for (i, n)
        f2[i] = f1[i] * f1[i], f3[i] = f2[i] * f1[i];
    _for (i, m)
        g2[i] = g1[i] * g1[i], g3[i] = g2[i] * g1[i];
    auto h1 = middle_product(f1, g3);
    auto h2 = middle_product(f2, g2);
    auto h3 = middle_product(f3, g1);
    vcb ans(n - m + 1);
    _for (i, n - m + 1)
        ans[i] = h1[i] == h2[i] + h2[i] - h3[i];
    return ans;
}