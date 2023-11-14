#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include "all.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    auto b = cumsum<i64>(a);
    _for (q) {
        dR(int, l, r);
        print(b[r] - b[l]);
    }
    return 0;
}