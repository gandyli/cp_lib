#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include "all.hpp"
#include "string/rolling_hash.hpp"

int main() {
    dR(str, s);
    Rollinghash hash(s);
    _for (i, len(s)) {
        io.write(Rollinghash::lcp(hash, 0, len(s), hash, i, len(s)), ' ');
    }
    writeln();
    return 0;
}