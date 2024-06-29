#define PROBLEM "https://atcoder.jp/contests/abc339/tasks/abc339_f"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "modint/mint61.hpp"
#include "random/hash.hpp"
#include "ds/counter.hpp"

template <typename mint>
mint to_mint(const str& s) {
    mint x = 0;
    foreach (c, s)
        x = x * 10 + c - '0';
    return x;
}
int main() {
    using mint1 = MMInt998244353;
    using mint2 = MMInt1000000007;
    using mint3 = MInt61;
    using mint4 = MMInt<20070803>;

    dR(int, n);
    vc<mint1> a1(n);
    vc<mint2> a2(n);
    vc<mint3> a3(n);
    vc<mint4> a4(n);
    vc<u64> a(n);
    _for (i, n) {
        dR(str, s);
        a1[i] = to_mint<mint1>(s);
        a2[i] = to_mint<mint2>(s);
        a3[i] = to_mint<mint3>(s);
        a4[i] = to_mint<mint4>(s);
        a[i] = hash(vc<u64>{a1[i].val(), a2[i].val(), a3[i].val(), a4[i].val()});
    }
    Counter<u64> C(a);
    int ans = 0;
    _for (i, n) {
        _for (j, n) {
            auto x1 = a1[i] * a1[j];
            auto x2 = a2[i] * a2[j];
            auto x3 = a3[i] * a3[j];
            auto x4 = a4[i] * a4[j];
            auto x = hash(vc<u64>{x1.val(), x2.val(), x3.val(), x4.val()});
            ans += C[x];
        }
    }
    print(ans);
    return 0;
}