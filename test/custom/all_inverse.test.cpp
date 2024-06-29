#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "random/base.hpp"
#include "modint/all_inverse.hpp"
#include "modint/montgomery.hpp"
#include "modint/mint61.hpp"
#include "modint/barrett.hpp"

template <typename mint>
void test0() {
    vi N{1, 2, 3, 4, 5, 100000};
    foreach (n, N) {
        vc<mint> a(n);
        _for (i, n)
            a[i] = rnd(1, mint::mod());
        auto b = all_inverse(a);
        _for (i, n)
            assert(a[i] * b[i] == 1);
    }
}
void test() {
    test0<MMInt998244353>();
    test0<MMInt1000000007>();
    test0<MInt61>();
    test0<MMInt64<909090909090909091>>();
    {
        SetBMod(u32, 998244353);
        test0<mint>();
    }
    {
        SetBMod(u64, 909090909090909091);
        test0<mint>();
    }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}