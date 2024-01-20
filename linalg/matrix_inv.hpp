#pragma once
#include "template.hpp"

template <typename T>
std::pair<vvc<T>, T> matrix_inv(vvc<T> a) {
    const int n = len(a);
    ASSERT(n == len(a[0]));
    VEC(T, b, n, n);
    _for (i, n)
        b[i][i] = 1;
    T det = 1;
    _for (i, n) {
        _for (j, i, n)
            if (a[j][i] != 0) {
                if (i != j) {
                    swap(a[i], a[j]);
                    swap(b[i], b[j]);
                    det = -det;
                }
                break;
            }
        if (a[i][i] == 0)
            return {};
        det *= a[i][i];
        T inv = 1 / a[i][i];
        _for (j, i, n)
            a[i][j] *= inv;
        _for (j, n)
            b[i][j] *= inv;
        _for (j, n)
            if (i != j) {
                T c = a[j][i];
                _for (k, i, n)
                    a[j][k] -= a[i][k] * c;
                _for (k, n)
                    b[j][k] -= b[i][k] * c;
            }
    }
    return {std::move(b), det};
}