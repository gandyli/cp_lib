#pragma once
#include "../template.hpp"

vi zalgorithm(const auto& s) {
    int n = len(s);
    if (n == 0)
        return {};
    vi z(n);
    z[0] = 0;
    for (int i = 1, j = 0; i < n; i++) {
        int& k = z[i];
        if (j + z[j] > i)
            k = Min(j + z[j] - i, z[i - j]);
        while (i + k < n && s[k] == s[i + k])
            k++;
        if (j + z[j] < i + z[i])
            j = i;
    }
    z[0] = n;
    return z;
}