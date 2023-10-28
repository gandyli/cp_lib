#define PROBLEM "https://judge.yosupo.jp/problem/addition_of_big_integers"

#include "all.hpp"
#include "math/bigint.hpp"

int main() {
    multipleTests([&] {
        dR(bigint, a, b);
        print(a + b);
    });
    return 0;
}