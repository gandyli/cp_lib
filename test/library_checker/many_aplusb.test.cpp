#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb"

#include "all.hpp"

int main() {
    multipleTests([&] {
        dR(i64, a, b);
        writeln(a + b);
    });
    return 0;
}