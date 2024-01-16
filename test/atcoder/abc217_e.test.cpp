#define PROBLEM "https://atcoder.jp/contests/abc217/tasks/abc217_e"

#include "all.hpp"
#include "ds/sortable_array.hpp"

int main() {
    dR(int, n);
    Sortable_Array<2 * ten(7)> sa(inf<int>, vi(n));
    int l = 0, r = 0;
    _for (n) {
        dR(int, t);
        if (t == 1) {
            dR(int, x);
            sa.set(r++, x);
        }
        else if (t == 2) {
            print(sa.get(l++));
        }
        else {
            sa.sort(l, r);
        }
    }
    return 0;
}