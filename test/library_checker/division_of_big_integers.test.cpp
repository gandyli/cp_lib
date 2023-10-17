#define PROBLEM "https://judge.yosupo.jp/problem/division_of_big_integers"

#include "all.hpp"
#include "math/bigint.hpp"


int main() {
    multipleTests([&]{
        dR(bigint, a, b);
        writeln(divmod(a, b));
    });
    return 0;
}