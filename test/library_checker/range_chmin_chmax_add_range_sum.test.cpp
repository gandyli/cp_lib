#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"

#include "all.hpp"
#include "ds/beats_summinmax_addchminchmax.hpp"

int main() {
    dR(u32, n, q);
    Beats_SumMinMax_AddChminChmax<i64> seg(n, [&](int) { return io.read<i64>(); });
    _for (q) {
        dR(u32, t, l, r);
        if (t == 0) {
            dR(i64, x);
            seg.chmin(l, r, x);
        }
        else if (t == 1) {
            dR(i64, x);
            seg.chmax(l, r, x);
        }
        else if (t == 2) {
            dR(i64, x);
            seg.add(l, r, x);
        }
        else
            print(seg.prod(l, r).sum);
    }
    return 0;
}