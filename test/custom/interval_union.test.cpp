#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "random/base.hpp"
#include "utility/interval_union.hpp"

void test(int n) {
    vi v(n + 1);
    Vec<pi> a;
    _for (Min(rnd(0, n / 2), rnd(0, n / 2))) {
        int l, r;
        do {
            l = rnd(0, n);
            r = rnd(0, n + 1);
        } while (l > r || std::abs(l - r) > 10);
        a.eb(l, r);
        _for (i, l, r)
            v[i] = 1;
    }
    auto ret = interval_union(a);
    _for (i, len(ret) - 1)
        assert(ret[i].second < ret[i + 1].first);
    for (auto&& [l, r]: ret)
        _for (i, l, r) {
            assert(v[i]);
            v[i] = 0;
        }
    assert(!sum(v));
}
void test() {
    _for (i, 100000)
        test(10);
    _for (i, 10000)
        test(100);
    _for (i, 1000)
        test(1000);
}
int main() {
    test();

    dR(int, v, b);
    print(v + b);
    return 0;
}