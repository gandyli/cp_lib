#pragma once
#include "poly/ntt.hpp"
#include "poly/convolution_karatsuba.hpp"

template <Modint mint>
vc<mint> convolution_ntt(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    int k = get_lg(n + m - 1), sz = 1 << k;
    vc<mint> s(sz);
    _for (i, len(a))
        s[i] = a[i];
    fft4(s, k);
    if (a == b)
        _for (i, sz)
            s[i] *= s[i];
    else {
        vc<mint> t(sz);
        _for (i, len(b))
            t[i] = b[i];
        fft4(t, k);
        _for (i, sz)
            s[i] *= t[i];
    }
    ifft4(s, k);
    s.resize(n + m - 1);
    mint iv = mint(sz).inv();
    foreach (x, s)
        x *= iv;
    return s;
}
template <Modint mint>
vc<mint> convolution_garner(const vc<mint>& a, const vc<mint>& b) {
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
    auto c0 = convolution_ntt(a0, b0);
    auto c1 = convolution_ntt(a1, b1);
    auto c2 = convolution_ntt(a2, b2);
    constexpr u64 W1 = w1 % mint::mod();
    constexpr u64 W2 = w2 % mint::mod();
    vc<mint> r(n + m - 1);
    crt<u64, W1, W2>(c0, c1, c2, r);
    return r;
}
template <typename R>
vc<double> convolution_fft(const vc<R>& a, const vc<R>& b);
template <Integer T, typename U = i64>
vc<U> convolution(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 2500)
        return convolution_naive(a, b);
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    _for (i, n) {
        a0[i] = a[i];
        a1[i] = a[i];
        a2[i] = a[i];
    }
    _for (i, m) {
        b0[i] = b[i];
        b1[i] = b[i];
        b2[i] = b[i];
    }
    auto c0 = convolution_ntt(a0, b0);
    auto c1 = convolution_ntt(a1, b1);
    auto c2 = convolution_ntt(a2, b2);
    vc<U> r(n + m - 1);
    crt<U, w1, w2>(c0, c1, c2, r);
    return r;
}
template <Modint mint>
vc<mint> convolution(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (__builtin_ctz(mint::mod() - 1) < 3 || n + m - 1 > (1 << __builtin_ctz(mint::mod() - 1))) {
        if (min(n, m) <= 200)
            return convolution_karatsuba(a, b);
        return convolution_garner(a, b);
    }
    if (min(n, m) <= 50)
        return convolution_karatsuba(a, b);
    return convolution_ntt(a, b);
}