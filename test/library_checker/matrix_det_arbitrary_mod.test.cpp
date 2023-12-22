#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"

#include "all.hpp"
#include "linalg/matrix_det.hpp"

int main() {
    dR(int, n, m);
    dRV(u32, a, n, n);
    print(matrix_det_mod(a, m));
    return 0;
}