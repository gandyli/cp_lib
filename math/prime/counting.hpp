#pragma once
#include "math/isqrt.hpp"

u64 prime_counting(u64 n) {
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    const int v = isqrt(n);
    int s = (v + 1) / 2;
    int smalls[s];
    _for (i, s)
        smalls[i] = i;
    int roughs[s];
    _for (i, s)
        roughs[i] = 2 * i + 1;
    i64 larges[s];
    _for (i, s)
        larges[i] = (n / (2 * i + 1) - 1) / 2;
    bool skip[v + 1];
    memset(skip, 0, sizeof(skip));
    int pc = 0;
    _for (p, 3, v + 1, 2)
        if (!skip[p]) {
            int q = p * p;
            if (i64(q) * q > n)
                break;
            skip[p] = true;
            _for (i, q, v + 1, 2 * p)
                skip[i] = true;
            int ns = 0;
            _for (k, s) {
                int i = roughs[k];
                if (skip[i])
                    continue;
                i64 d = i64(i) * p;
                larges[ns] = larges[k] - (d <= v ? larges[smalls[d >> 1] - pc] : smalls[((int(f64(n) / d)) - 1) >> 1]) + pc;
                roughs[ns++] = i;
            }
            s = ns;
            for (int i = (v - 1) >> 1, j = ((v / p) - 1) | 1; j >= p; j -= 2) {
                int t = smalls[j >> 1] - pc;
                while (i >= (j * p) >> 1)
                    smalls[i--] -= t;
            }
            pc++;
        }
    larges[0] += (i64)(s + 2 * (pc - 1)) * (s - 1) / 2;
    _for (i, 1, s)
        larges[0] -= larges[i];
    _for (l, 1, s) {
        i64 q = roughs[l];
        i64 M = n / q;
        int e = smalls[(M / q - 1) >> 1] - pc;
        if (e < l + 1)
            break;
        i64 t = 0;
        _for (k, l + 1, e + 1)
            t += smalls[(int(f64(M) / (roughs[k])) - 1) >> 1];
        larges[0] += t - i64(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}