#pragma once
#include "poly/ntt.hpp"
#include "poly/arbitrary_ntt.hpp"

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
    int k = get_lg(n + m - 1), sz = 1 << k;
#ifdef NTT_AVX2
    if constexpr (sizeof(mint) == 4) {
        U32Aligned f(sz), g(sz);
        _for (i, n)
            f[i] = a[i].val();
        memset(f + n, 0, (sz - n) << 2);
        _for (i, m)
            g[i] = b[m - i - 1].val();
        memset(g + m, 0, (sz - m) << 2);
        conv<mint>(f, g, sz);
        vc<mint> r(n - m + 1);
        _for (i, n - m + 1)
            r[i] = mint::from_int(f[i + m - 1]);
        return r;
    }
#endif
    vc<mint> f(sz);
    _for (i, n)
        f[i] = a[i];
    vc<mint> g(sz);
    _for (i, m)
        g[i] = b[m - i - 1];
    fft4(f, k);
    fft4(g, k);
    _for (i, sz)
        f[i] *= g[i];
    ifft4(f, k);
    f.resize(n);
    f.erase(f.begin(), f.begin() + m - 1);
    mint iv = mint(sz).inv();
    foreach (x, f)
        x *= iv;
    return f;
}
template <Modint mint>
vc<mint> middle_product_garner(const vc<mint>& a, const vc<mint>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
#ifdef NTT_AVX2
    int k = get_lg(n + m - 1), sz = 1 << k;
    U32Aligned f0(sz), f1(sz), f2(sz), g0(sz), g1(sz), g2(sz);
    _for (i, n) {
        f0[i] = val(a[i]) % mint0::mod();
        f1[i] = val(a[i]) % mint1::mod();
        f2[i] = val(a[i]) % mint2::mod();
    }
    memset(f0 + n, 0, (sz - n) << 2);
    memset(f1 + n, 0, (sz - n) << 2);
    memset(f2 + n, 0, (sz - n) << 2);
    _for (i, m) {
        g0[i] = val(b[m - i - 1]) % mint0::mod();
        g1[i] = val(b[m - i - 1]) % mint1::mod();
        g2[i] = val(b[m - i - 1]) % mint2::mod();
    }
    memset(g0 + m, 0, (sz - m) << 2);
    memset(g1 + m, 0, (sz - m) << 2);
    memset(g2 + m, 0, (sz - m) << 2);
    conv<mint0>(f0, g0, sz);
    conv<mint1>(f1, g1, sz);
    conv<mint2>(f2, g2, sz);
    auto c0 = f0 + m - 1, c1 = f1 + m - 1, c2 = f2 + m - 1;
#else
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
#endif
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
    if constexpr (StaticModint<mint> && __builtin_ctz(mint::mod() - 1) >= 20)
        return middle_product_ntt(a, b);
    return middle_product_garner(a, b);
}