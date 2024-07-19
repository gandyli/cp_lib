#define PROBLEM "https://judge.yosupo.jp/problem/characteristic_polynomial"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "linalg/characteristic_poly.hpp"

int main() {
    dR(int, n);
    using mint = MMInt998244353;
    dRV(mint, a, n, n);
    print(characteristic_poly(std::move(a)));
    return 0;
}