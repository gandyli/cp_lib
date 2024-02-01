#pragma once
#include "template.hpp"

template <typename T>
struct Rollback_Array {
    vc<T> a;
    vc<std::pair<int, T>> his;
    int n;
    Rollback_Array() = default;
    Rollback_Array(int n, T x = {}): a(n, x), n(n) {}
    Rollback_Array(const vc<T>& a): a(a), n(len(a)) {}
    int time() const { return len(his); }
    void rollback(int t) {
        _for_r (i, t, time()) {
            auto [x, y] = pop(his);
            a[x] = y;
        }
    }
    void set(int x, T y) {
        his.eb(x, a[x]);
        a[x] = y;
    }
    T get(int x) const { return a[x]; }
    T operator[](int x) const { return get(x); }
};