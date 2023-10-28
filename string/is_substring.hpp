#pragma once
#include "string/zalgorithm.hpp"

template <typename T>
bool is_substring(T s, const T& t) {
    int n = len(s), m = len(t);
    s.insert(s.end(), all(t));
    auto z = zalgorithm(s);
    _for (i, n, n + m)
        if (z[i] >= n)
            return true;
    return false;
}