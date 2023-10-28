#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_D"

#include "all.hpp"
#include "math/euler_phi.hpp"

int main() {
    dR(u32, n);
    print(euler_phi(n));
    return 0;
}