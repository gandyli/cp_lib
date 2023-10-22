#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"

#include "all.hpp"
#include "math/prime/sieve.hpp"

int main() {
    dR(int, n, a, b);
    auto primes = prime_sieve(n);
    vi ans;
    while (b < len(primes))
        ans.eb(primes[b]), b += a;
    writeln(len(primes), ' ', len(ans));
    writeln(ans);
    return 0;
}