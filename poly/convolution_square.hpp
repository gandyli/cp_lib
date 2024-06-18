#pragma once
#include "poly/ntt.hpp"
#include "poly/arbitrary_ntt.hpp"
#include "poly/convolution_karatsuba.hpp"

template <Modint mint>
vc<mint> convolution_square_ntt(const vc<mint>& a) {
    int n = len(a);
    int k = get_lg(n + n - 1), sz = 1 << k;
#ifdef NTT_AVX2
    if constexpr (sizeof(mint) == 4) {
        U32Aligned f(sz);
        _for (i, n)
            f[i] = a[i].val();
        memset(f + n, 0, (sz - n) << 2);
        conv<mint>(f, sz);
        vc<mint> r(n + n - 1);
        _for (i, n + n - 1)
            r[i] = mint::from_int(f[i]);
        return r;
    }
#endif
    vc<mint> f(sz);
    _for (i, n)
        f[i] = a[i];
    fft4(f, k);
    _for (i, sz)
        f[i] *= f[i];
    ifft4(f, k);
    f.resize(n + n - 1);
    mint iv = mint(sz).inv();
    foreach (x, f)
        x *= iv;
    return f;
}
template <typename T, typename U = T>
vc<U> convolution_square_garner(const vc<T>& a) {
    using namespace ArbitraryNTT;
    int n = len(a);
#ifdef NTT_AVX2
    int k = get_lg(n + n - 1), sz = 1 << k;
    U32Aligned f0(sz), f1(sz), f2(sz);
    _for (i, n) {
        f0[i] = val(a[i]) % mint0::mod();
        f1[i] = val(a[i]) % mint1::mod();
        f2[i] = val(a[i]) % mint2::mod();
    }
    memset(f0 + n, 0, (sz - n) << 2);
    memset(f1 + n, 0, (sz - n) << 2);
    memset(f2 + n, 0, (sz - n) << 2);
    conv<mint0>(f0, sz);
    conv<mint1>(f1, sz);
    conv<mint2>(f2, sz);
#else
    vc<mint0> a0(n);
    vc<mint1> a1(n);
    vc<mint2> a2(n);
    _for (i, n)
        a0[i] = val(a[i]), a1[i] = val(a[i]), a2[i] = val(a[i]);
    auto f0 = convolution_square_ntt(a0);
    auto f1 = convolution_square_ntt(a1);
    auto f2 = convolution_square_ntt(a2);
#endif
    vc<U> r(n + n - 1);
    if constexpr (Modint<T>)
        crt<u64>(f0, f1, f2, r, w1 % T::mod(), w2 % T::mod());
    else
        crt<U>(f0, f1, f2, r, w1, w2);
    return r;
}
template <Integer T, typename U = i64>
vc<U> convolution_square(const vc<T>& a) {
    using namespace ArbitraryNTT;
    int n = len(a);
    if (!n)
        return {};
    if (n <= 100)
        return convolution_karatsuba<T, U>(a, a);
    return convolution_square_garner<T, U>(a);
}
template <Modint mint>
vc<mint> convolution_square(const vc<mint>& a) {
    int n = len(a);
    if (!n)
        return {};
    if (n > 40)
        if constexpr (StaticModint<mint> && __builtin_ctzll(mint::mod() - 1) >= 20)
            return convolution_square_ntt(a);
    return n <= 40 ? convolution_karatsuba(a, a) : convolution_square_garner(a);
}