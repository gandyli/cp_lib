#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "linalg/matrix_inv.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n);
    dRV(mint, a, n, n);
    auto [b, det] = matrix_inv(a);
    if (det != 0)
        io.displayArray(b, '\n');
    else
        print(-1);
    return 0;
}