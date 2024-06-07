#define PROBLEM "https://judge.yosupo.jp/problem/wildcard_pattern_matching"

#include "all.hpp"
#include "poly/ntt_avx2.hpp"
#include "string/wildcard_pattern_matching.hpp"

int main() {
    dR(str, s, t);
    foreach (x, wildcard_pattern_matching(s, t, '*'))
        io.putch('0' + x);
    print();
    return 0;
}