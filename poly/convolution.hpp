#pragma once
#include "poly/ntt.hpp"
#include "poly/fft.hpp"
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
template <typename T, typename U = T>
vc<U> convolution_garner(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    vc<mint0> a0(n), b0(m);
    vc<mint1> a1(n), b1(m);
    vc<mint2> a2(n), b2(m);
    auto val = [&](auto&& a) {
        if constexpr (Modint<T>)
            return a.val();
        else
            return a;
    };
    _for (i, n) {
        a0[i] = val(a[i]);
        a1[i] = val(a[i]);
        a2[i] = val(a[i]);
    }
    _for (i, m) {
        b0[i] = val(b[i]);
        b1[i] = val(b[i]);
        b2[i] = val(b[i]);
    }
    auto c0 = convolution_ntt(a0, b0);
    auto c1 = convolution_ntt(a1, b1);
    auto c2 = convolution_ntt(a2, b2);
    vc<U> r(n + m - 1);
    if constexpr (Modint<T>)
        crt<u64>(c0, c1, c2, r, w1 % T::mod(), w2 % T::mod());
    else
        crt<U>(c0, c1, c2, r, w1, w2);
    return r;
}
template <typename R, typename U = f64, bool ROUND = false>
vc<U> convolution_fft(const vc<R>& a, const vc<R>& b) {
    using namespace CFFT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 60)
        return convolution_naive<R, U>(a, b);
    int k = get_lg(n + m - 1), sz = 1 << k;
    vc<C> c(sz);
    _for (i, n)
        c[i].x = a[i];
    _for (i, m)
        c[i].y = b[i];
    setw(k);
    fft(c, k);
    c[0].y = 4 * c[0].x * c[0].y;
    c[1].y = 4 * c[1].x * c[1].y;
    c[0].x = c[1].x = 0;
    _for (i, 2, sz, 2) {
        int j = i ^ (std::__bit_floor(i) - 1);
        c[i] = (c[i] + c[j].conj()) * (c[i] - c[j].conj());
        c[j] = -c[i].conj();
    }
    _for (i, sz >> 1) {
        C A0 = c[i << 1] + c[i << 1 | 1];
        C A1 = (c[i << 1] - c[i << 1 | 1]) * w[i].conj();
        c[i] = A0 + A1.rotl();
    }
    ifft(c, k - 1);
    vc<U> ret(n + m - 1);
    auto round = [](f64 x) { return x > 0 ? x + 0.5 : x - 0.5; };
    _for (i, n + m - 1)
        if constexpr (ROUND)
            ret[i] = round((i & 1 ? -c[i >> 1].x : c[i >> 1].y) / (4 * sz));
        else
            ret[i] = (i & 1 ? -c[i >> 1].x : c[i >> 1].y) / (4 * sz);
    return ret;
}
template <Integer T, typename U = i64>
vc<U> convolution(const vc<T>& a, const vc<T>& b) {
    using namespace ArbitraryNTT;
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if (min(n, m) <= 900)
        return convolution_karatsuba<T, U>(a, b);
    return convolution_garner<T, U>(a, b);
}
template <Modint mint>
vc<mint> convolution(const vc<mint>& a, const vc<mint>& b) {
    int n = len(a), m = len(b);
    if (!n || !m)
        return {};
    if constexpr (StaticModint<mint>) {
        constexpr int lvl = __builtin_ctz(mint::mod() - 1);
        if (n + m - 1 <= (1 << lvl) && min(n, m) > 40)
            return convolution_ntt(a, b);
    }
    return min(n, m) <= 340 ? convolution_karatsuba(a, b) : convolution_garner(a, b);
}