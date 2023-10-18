#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"

#include "all.hpp"
#include "utility/pdqsort.hpp"

int main() {
    dR(int, n);
    dRV(pi, a, n);
    pdqsort(all(a), [&](auto&& x, auto&& y) {
        return atan2l(x.second, x.first) < atan2l(y.second, y.first);
    });
    io.displayArray(a, '\n');
    return 0;
}