#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "utility/radix_sort.hpp"
#include "random/base.hpp"

template <typename T>
void test0(int n) {
    T min = Unsigned<T> ? 0 : -inf<T>, max = inf<T>;
    {
        vc<T> v(n);
        foreach (x, v)
            x = rnd(min, max);
        auto w = v;
        radix_sort(v);
        sort(w);
        assert(v == w);
    }
    vc<std::pair<T, T>> v(n);
    foreach (x, v) {
        x.first = rnd(min, max);
        x.second = rnd(min, max);
    }
    auto w = v;
    radix_sort_compare_first(v);
    stable_sort(w, [](auto&& a, auto&& b) {
        return a.first < b.first;
    });
    assert(v == w);
}
void test() {
    {
        vi v{2, 0, 1, -1, -2};
        radix_sort(v);
        vi w{-2, -1, 0, 1, 2};
        assert(v == w);
    }
    {
        vi v{2, 0, 2, -1, 1, 2, 0, -1, -2};
        radix_sort(v);
        vi w{-2, -1, -1, 0, 0, 1, 2, 2, 2};
        assert(v == w);
    }
    {
        vc<i64> v{2, 0, 2, -1, 1, 2, 0, -1, -2};
        radix_sort(v);
        vc<i64> w{-2, -1, -1, 0, 0, 1, 2, 2, 2};
        assert(v == w);
    }

    _for (n, 200)
        _for (200) {
            test0<int>(n);
            test0<u32>(n);
            test0<i64>(n);
        }
    _for (e, 6) {
        test0<int>(ten(e));
        test0<u32>(ten(e));
        test0<i64>(ten(e));
    }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}