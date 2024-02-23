#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "poly/convolution_fft.hpp"
#include "poly/convolution.hpp"
#include "random/base.hpp"

void test() {
    vi N{1, 10, 100, 1000, 10000};
    foreach (n, N) {
        vi a(n), b(n);
        _for (i, n)
            a[i] = rnd(0, 10);
        _for (i, n)
            b[i] = rnd(0, 10);
        auto c = convolution_fft<int, int, true>(a, b);
        auto d = convolution<int, int>(a, b);
        assert(c == d);
    }
    N = {5 * ten(5), ten(6)};
    foreach (n, N) {
        vi a(n), b(n);
        _for (i, n)
            a[i] = rnd(0, 10);
        _for (i, n)
            b[i] = rnd(0, 10);
        auto c = convolution_fft<int, i64, true>(a, b);
        auto d = convolution<int, i64>(a, b);
        assert(c == d);
    }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}