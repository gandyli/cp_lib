#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"

#include "all.hpp"
#include "modint/mod_log.hpp"

int main() {
    multipleTests([&] {
        dR(u32, x, y, p);
        print(int(mod_log(x, y, p).value_or(-1)));
    });
    return 0;
}