#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "linalg/matrix_det.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n);
    dRV(mint, a, n, n);
    print(matrix_det(a));
    return 0;
}