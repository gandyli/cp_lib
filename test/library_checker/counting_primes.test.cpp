#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"

#include "all.hpp"
#include "prime/prime_counting.hpp"

int main() {
    dR(i64, n);
    writeln(prime_counting(n));
    return 0;
}