#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_A"

#include "all.hpp"
#include "ds/swag.hpp"
#include "monoid/add.hpp"

int main() {
    dR(int, n, s);
    dRV(int, a, n);
    SWAG<Monoid_Add<int>> swag;
    int ans = inf<int>;
    foreach (x, a) {
        swag.push(x);
        while (swag.prod() >= s) {
            chkmin(ans, len(swag));
            swag.pop();
        }
    }
    print(ans == inf<int> ? 0 : ans);
    return 0;
}