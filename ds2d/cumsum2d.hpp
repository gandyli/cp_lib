#pragma once
#include "../template.hpp"

template <typename T>
struct Cumsum2D {
    Vec<Vec<T>> s;
    Cumsum2D(const Vec<Vec<T>>& a) {
        int n = len(a), m = len(a[0]);
        s.assign(n + 1, Vec<T>(m + 1));
        _for (i, n)
            _for (j, m)
                s[i + 1][j + 1] = a[i][j];
        _for (i, n + 1)
            _for (j, m)
                s[i][j + 1] += s[i][j];
        _for (i, n)
            _for (j, m + 1)
                s[i + 1][j] += s[i][j];
    }
    auto sum(int x1, int y1, int x2, int y2) { return s[x1][y1] - s[x1][y2] - s[x2][y1] + s[x2][y2]; }
};