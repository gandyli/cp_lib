#define PROBLEM "https://judge.yosupo.jp/problem/longest_increasing_subsequence"

#include "all.hpp"
#include "seq/longest_increasing_subsequence.hpp"

int main() {
    dR(int, n);
    dRV(int, a, n);
    auto I = longest_increasing_sequence_id(a);
    print(len(I));
    print(I);
    return 0;
}