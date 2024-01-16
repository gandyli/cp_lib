#define PROBLEM "https://atcoder.jp/contests/abc237/tasks/abc237_g"

#include "all.hpp"
#include "ds/sortable_array.hpp"

int main() {
    dR(int, n, q, x);
    dRV(int, a, n);
    Sortable_Array<ten(7)> sa(n + 1, a);
    _for (q) {
        dR(int, t, l, r), l--;
        sa.sort(l, r, t == 2);
    }
    a = sa.get_all();
    print(find(a, x) - a.begin() + 1);
    return 0;
}