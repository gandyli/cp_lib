#define PROBLEM "https://judge.yosupo.jp/problem/number_of_subsequences"

#include "all.hpp"
#include "modint/montgomery.hpp"
#include "string/count_subsequence.hpp"

using mint = MMInt998244353;
int main() {
    dR(int, n);
    dRV(int, a, n);
    print(count_subsequence<mint>(a));
    return 0;
}