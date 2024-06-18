#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "poly/ntt_avx2.hpp"
#include "poly/convolution.hpp"
#include "poly/convolution_square.hpp"
#include "random/base.hpp"

template <typename mint>
void test0() {
    vi N{1, 10, 100, 1000, 10000, ten(5)};
    foreach (n, N) {
        vc<mint> a(n);
        _for (i, n)
            a[i] = rnd(0, mint::mod());
        auto f = convolution_square(a);
        auto g = convolution(a, a);
        assert(f == g);
    }
}
void test1() {
    vi N{1, 10, 100, 1000, 10000, ten(5)};
    foreach (n, N) {
        vi a(n);
        _for (i, n)
            a[i] = rnd(0, ten(9));
        auto f = convolution_square<int, u128>(a);
        auto g = convolution<int, u128>(a, a);
        assert(f == g);
    }
}
void test() {
    test0<MMInt998244353>();
    test0<MMInt1000000007>();
    test1();
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}