#define PROBLEM "https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial"

#include "all.hpp"
#include "poly/lagrange_interpolate_iota.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n, m, x);
    dRV(mint, a, n);
    print(lagrange_interpolate_iota<mint>(a, x, m));
    return 0;
}