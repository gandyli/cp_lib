#pragma once
#include "linalg/frobenius.hpp"

template <Integer T>
vvc<T> matrix_pow(vvc<T> a, auto k) {
    const int n = len(a);
    ASSERT(n == len(a[0]));
    VEC(T, ans, n, n);
    _for (i, n)
        ans[i][i] = 1;
    for (; k; k >>= 1, a *= a)
        if (k & 1)
            ans *= a;
    return ans;
}
template <Modint mint>
vvc<mint> matrix_pow(const vvc<mint>& a, auto k) { return Frobenius(a)(k); }