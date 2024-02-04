#pragma once
#include "linalg/transpose.hpp"

template <typename T>
vvc<T> matrix_mul(const vvc<T>& a, const vvc<T>& b) {
    int n0 = len(a), n1 = len(b), n2 = len(b[0]);
    ASSERT(n1 == len(a[0]));
    auto t = transpose(b);
    VEC(T, c, n0, n2);
    _for (i, n0)
        _for (j, n2)
            _for (k, n1)
                c[i][j] += a[i][k] * t[j][k];
    return c;
}