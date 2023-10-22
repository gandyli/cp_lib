#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include "all.hpp"
#include "math/factorize.hpp"

int main() {
    multipleTests([&]() {
        dR(u64, n);
        auto ans = factorize(n);
        writeln(len(ans), ' ', ans);
    });
    return 0;
}