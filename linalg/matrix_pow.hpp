#pragma once
#include "linalg/matrix_mul.hpp"

template <typename T>
vvc<T> matrix_pow(vvc<T> a, auto b) {
    const int n = len(a);
    ASSERT(n == len(a[0]));
    VEC(T, ans, n, n);
    _for (i, n)
        ans[i][i] = 1;
    for (; b; b >>= 1, a = matrix_mul(a, a))
        if (b & 1)
            ans = matrix_mul(ans, a);
    return ans;
}