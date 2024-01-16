#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "random/base.hpp"
#include "ds/sortable_array.hpp"

void test() {
    int n = rnd(1, 64), m = rnd(2, 20);
    vi a(n);
    _for (i, n)
        a[i] = rnd(0, m);
    Sortable_Array<15000> X(m, a);
    int q = 1000;
    _for (q) {
        int t = rnd(0, 5);
        if (t == 0) {
            vi b = X.get_all();
            assert(a == b);
        }
        if (t == 1) {
            int i = rnd(0, n);
            assert(a[i] == X.get(i));
        }
        if (t == 2) {
            int i = rnd(0, n), x = rnd(0, m);
            a[i] = x;
            X.set(i, x);
        }
        if (t == 3) {
            int l = rnd(0, n), r = rnd(0, n);
            if (l > r)
                swap(l, r);
            r++;
            sort(a.begin() + l, a.begin() + r);
            X.sort(l, r);
        }
        if (t == 4) {
            int l = rnd(0, n), r = rnd(0, n);
            if (l > r)
                swap(l, r);
            r++;
            sort(a.begin() + l, a.begin() + r, std::greater{});
            X.sort(l, r, true);
        }
    }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}