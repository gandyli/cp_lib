#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include "all.hpp"
#include "math/prime/test.hpp"

int main() {
    multipleTests([&] {
        dR(u64, x);
        Yes(is_prime(x));
    });
    return 0;
}