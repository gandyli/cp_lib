#pragma once
#include "template.hpp"

template <typename T>
void to_Hessenberg(vvc<T>& a) {
    int n = len(a);
    _for (k, n - 2) {
        _for (i, k + 1, n)
            if (a[i][k] != 0) {
                if (i != k + 1) {
                    swap(a[i], a[k + 1]);
                    _for (j, n)
                        swap(a[j][i], a[j][k + 1]);
                }
                break;
            }
        if (a[k + 1][k] == 0)
            continue;
        _for (i, k + 2, n) {
            T x = a[i][k] / a[k + 1][k];
            _for (j, n)
                a[i][j] -= a[k + 1][j] * x;
            _for (j, n)
                a[j][k + 1] += a[j][i] * x;
        }
    }
}
template <typename T>
vc<T> characteristic_poly(vvc<T> a) {
    int n = len(a);
    to_Hessenberg(a);
    vvc<T> dp(n + 1);
    dp[0] = {1};
    _for (k, n) {
        dp[k + 1].resize(k + 2);
        auto& d = dp[k + 1];
        _for (i, k + 1)
            d[i + 1] += dp[k][i];
        _for (i, k + 1)
            d[i] -= a[k][k] * dp[k][i];
        T prod = 1;
        _for_r (i, k) {
            prod *= a[i + 1][i];
            T x = a[i][k] * prod;
            _for (j, i + 1)
                d[j] -= x * dp[i][j];
        }
    }
    return dp[n];
}