#pragma once
#include "modint/barrett_reduction.hpp"

u32 matrix_det_mod(Vec<Vec<u32>> a, u32 mod) {
    const int n = len(a);
    ASSERT(n == len(a[0]));

    Barrett<u32> bt(mod);
    u32 det = 1;
    _for (i, n) {
        _for (j, i, n)
            if (a[j][i]) {
                if (i != j) {
                    swap(a[i], a[j]);
                    det = mod - det;
                }
                break;
            }
        _for (j, i + 1, n) {
            while (a[i][i]) {
                u64 t = mod - a[j][i] / a[i][i];
                _for_r (k, i, n)
                    a[j][k] = bt.mod(a[j][k] + a[i][k] * t);
                swap(a[i], a[j]);
                det = mod - det;
            }
            swap(a[i], a[j]);
            det = mod - det;
        }
    }
    _for (i, n)
        det = bt.mul(det, a[i][i]);
    return det;
}
template <typename mint>
mint matrix_det(const Vec<Vec<mint>>& a) {
    const int n = len(a);
    VEC(u32, b, n, n);
    _for (i, n)
        _for (j, n)
            b[i][j] = a[i][j].val();
    return matrix_det_mod(b, mint::mod());
}