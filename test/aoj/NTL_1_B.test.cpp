#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_B"

#include "all.hpp"
#include "modint/montgomery.hpp"

using Z = MMInt1000000007;
int main() {
    dR(u32, m, n);
    print(power(Z(m), n));
    return 0;
}