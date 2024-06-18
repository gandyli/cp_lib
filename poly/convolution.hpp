#pragma once
#include "poly/ntt.hpp"
#include "poly/arbitrary_ntt.hpp"
#include "poly/convolution_karatsuba.hpp"

template <Modint mint>
vc<mint> convolution_ntt(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    int k = get_lg(n + m - 1), sz = 1 << k;
#ifdef NTT_AVX2
    if constexpr (sizeof(mint) == 4) {
        U32Aligned f(sz), g(sz);
        _for (i, n)
            f[i] = a[i].val();
        memset(f + n, 0, (sz - n) << 2);
        _for (i, m)
            g[i] = b[i].val();
        memset(g + m, 0, (sz - m) << 2);
        conv<mint>(f, g, sz);
        vc<mint> r(n + m - 1);
        _for (i, n + m - 1)
            r[i] = mint::from_int(f[i]);
        return r;
    }
#endif
    vc<mint> f(sz), g(sz);
    _for (i, n)
        f[i] = a[i];
    _for (i, m)
        g[i] = b[i];
    fft4(f, k);
    fft4(g, k);
    _for (i, sz)
        f[i] *= g[i];
    ifft4(f, k);
    f.resize(n + m - 1);
    mint iv = mint(sz).inv();
    foreach (x, f)
        x *= iv;
    return f;
}
template <typename T, typename U = T>
vc<U> convolution_garner(const vc<T>& a, const vc<T>& b) {
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
        g0[i] = val(b[i]) % mint0::mod();
        g1[i] = val(b[i]) % mint1::mod();
        g2[i] = val(b[i]) % mint2::mod();
    }
    memset(g0 + m, 0, (sz - m) << 2);
    memset(g1 + m, 0, (sz - m) << 2);
    memset(g2 + m, 0, (sz - m) << 2);
    conv<mint0>(f0, g0, sz);
    conv<mint1>(f1, g1, sz);
    conv<mint2>(f2, g2, sz);
#else
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    _for (i, n)
        a0[i] = val(a[i]), a1[i] = val(a[i]), a2[i] = val(a[i]);
    _for (i, m)
        b0[i] = val(b[i]), b1[i] = val(b[i]), b2[i] = val(b[i]);
    auto f0 = convolution_ntt(a0, b0);
    auto f1 = convolution_ntt(a1, b1);
    auto f2 = convolution_ntt(a2, b2);
#endif
    vc<U> r(n + m - 1);
    if constexpr (Modint<T>)
        crt<u64>(f0, f1, f2, r, w1 % T::mod(), w2 % T::mod());
    else
        crt<U>(f0, f1, f2, r, w1, w2);
    return r;
}
template <Integer T, typename U = i64>
vc<U> convolution(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 100)
        return convolution_karatsuba<T, U>(a, b);
    return convolution_garner<T, U>(a, b);
}
template <Modint mint>
vc<mint> convolution(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) > 40)
        if constexpr (StaticModint<mint> && __builtin_ctzll(mint::mod() - 1) >= 20)
            return convolution_ntt(a, b);
    return min(n, m) <= 40 ? convolution_karatsuba(a, b) : convolution_garner(a, b);
}