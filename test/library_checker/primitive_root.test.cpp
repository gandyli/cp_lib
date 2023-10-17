#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"

#include "all.hpp"
#include "math/primitive_root.hpp"

int main() {
    multipleTests([&] {
        dR(u64, x);
        writeln(primitive_root(x));
    });
    return 0;
}