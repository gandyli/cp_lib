#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "linalg/matrix_mul.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n, m, k);
    dRV(mint, a, n, m);
    dRV(mint, b, m, k);
    print(matrix_mul(a, b));
    return 0;
}