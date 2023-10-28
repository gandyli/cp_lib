#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_E"

#include "all.hpp"
#include "math/exgcd.hpp"

int main() {
    dR(int, a, b);
    auto [x, y, d] = exgcd(a, b);
    print(x, y);
    return 0;
}