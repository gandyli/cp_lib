#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_integer"

#include "all.hpp"
#include "math/inth_root.hpp"

int main() {
    multipleTests([&] {
        dR(u64, n, k);
        print(inth_root(n, k));
    });
    return 0;
}