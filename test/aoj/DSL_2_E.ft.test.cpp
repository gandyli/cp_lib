#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E"

#include "all.hpp"
#include "ds/range_add_range_sum.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    Range_Add_Range_Sum<Monoid_Add<int>> ft(n);
    _for (q) {
        dR(int, t, x);
        if (t == 0) {
            dR(int, y, z);
            ft.apply(x, y + 1, z);
        }
        else {
            print(ft.prod(x, x + 1));
        }
    }
    return 0;
}