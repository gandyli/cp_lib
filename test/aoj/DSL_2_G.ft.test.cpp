#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G"

#include "all.hpp"
#include "ds/range_add_range_sum.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, q);
    Range_Add_Range_Sum<Monoid_Add<i64>> ft(n);
    _for (q) {
        dR(int, t, l, r), l--;
        if (t == 0) {
            dR(int, x);
            ft.apply(l, r, x);
        }
        else {
            print(ft.prod(l, r));
        }
    }
    return 0;
}