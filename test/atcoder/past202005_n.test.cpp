#define PROBLEM "https://atcoder.jp/contests/past202005-open/tasks/past202005_n"

#include "all.hpp"
#include "ds/sortable_array.hpp"

int main() {
    dR(int, n, q);
    vi a(n);
    iota(all(a), 1);
    Sortable_Array<2 * ten(7)> sa(n + 1, a); 
    _for (q) {
        dR(int, t, x, y);
        if (t == 1) {
            x--;
            auto vx = sa.get(x), vy = sa.get(x + 1);
            sa.set(x, vy);
            sa.set(x + 1, vx);
        }
        else {
            x--;
            sa.sort(x, y);
        }
    }
    print(sa.get_all());
    return 0;
}