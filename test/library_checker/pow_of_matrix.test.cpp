#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_matrix"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "linalg/matrix_pow.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n);
    dR(i64, k);
    dRV(mint, a, n, n);
    print(matrix_pow(a, k));
    return 0;
}