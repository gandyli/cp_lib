#pragma once
#include "template.hpp"

int longest_common_subsequence(auto&& a, auto&& b) {
    int n = len(b);
    vi d(n + 1);
    foreach (a, a) {
        _for_r (i, n)
            if (a == b[i])
                chkmax(d[i + 1], d[i] + 1);
        _for (i, n)
            chkmax(d[i + 1], d[i]);
    }
    return d[n];
}
vc<pi> longest_common_subsequence_result(auto&& a, auto&& b) {
    int n = len(a), m = len(b);
    VEC(int, d, n + 1, m + 1);
    _for (i, n) {
        d[i + 1] = d[i];
        _for (j, m) {
            chkmax(d[i + 1][j + 1], d[i + 1][j]);
            if (a[i] == b[j])
                chkmax(d[i + 1][j + 1], d[i][j] + 1);
        }
    }
    vc<pi> r;
    int i = n, j = m;
    while (d[i][j])
        if (d[i][j] == d[i - 1][j])
            i--;
        else if (d[i][j] == d[i][j - 1])
            j--;
        else
            r.eb(--i, --j);
    reverse(r);
    return r;
}