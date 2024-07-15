#define PROBLEM "https://atcoder.jp/contests/abc362/tasks/abc362_g"

#include "all.hpp"
#include "string/suffix_array.hpp"

int main() {
    dR(str, S);
    dR(int, n);
    S += '$';
    vi cut{0};
    cut.eb(len(S));
    _for (n) {
        dR(str, t);
        S += t;
        cut.eb(len(S));
    }
    Suffix_Array SA(S);
    SA.build_lcp();
    vi a(len(S));
    _for (i, cut[1])
        a[SA.isa[i]] = 1;
    auto Ac = cumsum(a);
    _for (i, n) {
        int l = cut[i + 1], r = cut[i + 2];
        auto [x, y] = SA.lcp_range(l, r - l);
        print(Ac[y] - Ac[x]);
    }
    return 0;
}