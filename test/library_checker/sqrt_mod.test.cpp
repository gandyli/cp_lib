#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"

#include "all.hpp"
#include "modint/mod_sqrt.hpp"

int main() {
    multipleTests([&] {
        dR(u32, n, p);
        print(mod_sqrt(n, p));
    });
    return 0;
}