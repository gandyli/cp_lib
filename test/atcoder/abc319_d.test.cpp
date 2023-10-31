#define PROBLEM "https://atcoder.jp/contests/abc319/tasks/abc319_d"

#include "all.hpp"
#include "math/binary_search.hpp"

int main() {
    dR(int, n, m);
    dRV(int, a, n);

    auto check = [&](i64 x) {
        int c = 1;
        i64 s = -1;
        _for (i, n) {
            if (s + a[i] + 1 > x) {
                c++;
                s = -1;
            }
            s += a[i] + 1;
        }
        return c <= m;
    };
    print(bsearch<i64>(check, inf<i64>, max(a) - 1));
    return 0;
}