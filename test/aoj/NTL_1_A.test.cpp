#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_A"

#include "all.hpp"
#include "math/factorize.hpp"

int main() {
    dR(u32, n);
    writeln(n, ": ", factorize(n));
    return 0;
}