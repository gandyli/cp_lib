#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_C"

#include "all.hpp"
#include "ds/kdtree.hpp"

int main() {
    dR(int, n);
    vi X(n), Y(n);
    io.zipread(X, Y);
    KDTree tree(X, Y);
    dR(int, q);
    _for (q) {
        dR(int, xl, xr, yl, yr), xr++, yr++;
        auto I = tree.collect_rect(xl, xr, yl, yr);
        sort(I);
        foreach (i, I)
            print(i);
        print();
    }
    return 0;
}