#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"

#include "all.hpp"
#include "math/prime/counting.hpp"

int main() {
    dR(i64, n);
    writeln(prime_counting(n));
    return 0;
}