#pragma once
#include "template.hpp"

template <typename T>
struct BIT {
    int n;
    Vec<T> a;
    BIT(int n): n(n), a(n) {}
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i)
            a[i - 1] += v;
    }
    T sum(int x) {
        T ans = 0;
        for (int i = x + 1; i > 0; i -= i & -i)
            ans += a[i - 1];
        return ans;
    }
    T sum(int l, int r) {
        return sum(r - 1) - sum(l - 1);
    }
};
