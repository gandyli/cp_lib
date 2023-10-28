#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_C"

#include "all.hpp"
#include "math/lcm.hpp"

int main() {
    dR(int, n);
    dRV(int, a, n);
    print(lcm<int>(a));
    return 0;
}