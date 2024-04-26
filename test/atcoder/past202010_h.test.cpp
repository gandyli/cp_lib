#define PROBLEM "https://atcoder.jp/contests/past202010-open/tasks/past202010_h"

#include "all.hpp"
#include "ds/counter.hpp"

int main() {
    dR(int, n, m, k);
    dRV(str, s, n);
    _for_r (t, 1, min(n, m) + 1) {
        _for (i, n - t + 1) {
            _for (j, m - t + 1) {
                Counter<char> c;
                _for (x, t)
                    _for (y, t)
                        c[s[i + x][j + y]]++;
                if (t * t - c.most_common()[0].second <= k) {
                    print(t);
                    return 0;
                }
            }
        }
    }
    return 0;
}