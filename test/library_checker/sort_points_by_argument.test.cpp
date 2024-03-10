#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"

#include "all.hpp"

int main() {
    dR(int, n);
    dRV(pi, a, n);
    sort(a, [&](auto&& x, auto&& y) {
        return atan2l(x.second, x.first) < atan2l(y.second, y.first);
    });
    print(a);
    return 0;
}