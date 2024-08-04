#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "random/base.hpp"
#include "ds/index_compression.hpp"

template <bool SMALL>
void test_distinct() {
    vi X = {3, 4, 7, 4, 3, 6, 6, 7, 3};
    Index_Compression<int, false, SMALL> IDX;
    auto [Y, _] = IDX.build(X);
    assert(Y == vi({0, 3, 7, 4, 1, 5, 6, 8, 2}));
    assert(IDX(1) == 0 && IDX(2) == 0 && IDX(3) == 0);
    assert(IDX(4) == 3 && IDX(5) == 5 && IDX(6) == 5);
    assert(IDX(7) == 7 && IDX(8) == 9 && IDX(9) == 9);

    _for (10) {
        _for (N, 0, 300) {
            vi X(N);
            _for (i, N)
                X[i] = rnd(-100, 100);
            Index_Compression<int, false, SMALL> IDX;
            auto [Y, _] = IDX.build(X);
            auto I = argsort(X);
            _for (i, N)
                assert(Y[I[i]] == i);
            X = rearrange(X, I);
            _for (x, -100, 100)
                assert(IDX(x) == LB(X, x));
        }
    }
}
template <bool SMALL>
void test_same() {
    vi X = {3, 4, 7, 4, 3, 6, 6, 7, 3};
    Index_Compression<int, true, SMALL> IDX;
    auto [Y, _] = IDX.build(X);
    assert(Y == vi({0, 1, 3, 1, 0, 2, 2, 3, 0}));
    assert(IDX(1) == 0 && IDX(2) == 0 && IDX(3) == 0);
    assert(IDX(4) == 1 && IDX(5) == 2 && IDX(6) == 2);
    assert(IDX(7) == 3 && IDX(8) == 4 && IDX(9) == 4);

    _for (10) {
        _for (N, 0, 300) {
            vi X(N);
            _for (i, N)
                X[i] = rnd(-100, 100);
            Index_Compression<int, true, SMALL> IDX;
            auto [Y, _] = IDX.build(X);
            vi key = X;
            UNIQUE(key);
            _for (i, N)
                assert(LB(key, X[i]) == Y[i]);
            _for (x, -100, 100)
                assert(IDX(x) == LB(key, x));
        }
    }
}
void test() {
    test_distinct<false>();
    test_distinct<true>();
    test_same<false>();
    test_same<true>();
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}