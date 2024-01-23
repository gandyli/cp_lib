#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"

#include "all.hpp"
#include "modint/mod_sqrt.hpp"

int main() {
    multipleTests([&] {
        dR(u32, n, p);
        auto r = mod_sqrt(n, p);
        if (r)
            print(*r);
        else
            print("-1");
    });
    return 0;
}