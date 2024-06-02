#pragma once
#include "poly/ntt.hpp"

template <Modint mint>
vc<mint> middle_product_naive(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    vc<mint> r(n - m + 1);
    _for (i, n - m + 1)
        _for (j, m)
            r[i] += a[i + j] * b[j];
    return r;
}
template <Modint mint>
vc<mint> middle_product_ntt(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    int k = std::__lg(n * 2 - 1), sz = 1 << k;
    vc<mint> s(sz);
    _for (i, n)
        s[i] = a[i];
    vc<mint> t(sz);
    _for (i, m)
        t[i] = b[m - i - 1];
    fft4(s, k);
    fft4(t, k);
    _for (i, sz)
        s[i] *= t[i];
    ifft4(s, k);
    s.resize(n);
    s.erase(s.begin(), s.begin() + m - 1);
    mint iv = mint(sz).inv();
    foreach (x, s)
        x *= iv;
    return s;
}
template <Modint mint>
vc<mint> middle_product_garner(const vc<mint>& a, const vc<mint>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    _for (i, n) {
        a0[i] = a[i].val();
        a1[i] = a[i].val();
        a2[i] = a[i].val();
    }
    _for (i, m) {
        b0[i] = b[i].val();
        b1[i] = b[i].val();
        b2[i] = b[i].val();
    }
    auto c0 = middle_product_ntt(a0, b0);
    auto c1 = middle_product_ntt(a1, b1);
    auto c2 = middle_product_ntt(a2, b2);
    vc<mint> r(n - m + 1);
    crt<mint>(c0, c1, c2, r, w1, w2);
    return r;
}
template <Modint mint>
vc<mint> middle_product(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    ASSERT(n >= m);
    if (!m)
        return vc<mint>(n + 1);
    if (min(m, n - m + 1) <= 60)
        return middle_product_naive(a, b);
    if constexpr (StaticModint<mint>) {
        constexpr int lvl = __builtin_ctz(mint::mod() - 1);
        if (2 * n - 1 <= (1 << lvl))
            return middle_product_ntt(a, b);
    }
    return middle_product_garner(a, b);
}