#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include "all.hpp"
#include "ds/wavelet_matrix.hpp"

int main() {
    dR(int, n, m);
    dRV(int, a, n);
    Wavelet_Matrix<int, false> wm(a);
    _for (m) {
        dR(int, l, r, k);
        print(wm.kth(l, r, k));
    }
    return 0;
}