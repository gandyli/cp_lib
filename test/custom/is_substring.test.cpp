#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "string/is_substring.hpp"
#include "random/base.hpp"

str gen(int n) {
    str s;
    _for (n)
        s += char('a' + rnd(0, 3));
    return s;
}
void test() {
    _for (1000)
        _for (n, 1, 10)
            _for (m, 1, 10) {
                str s = gen(n), t = gen(m);
                assert(is_substring(s, t) == (t.find(s) != t.npos));
            }
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}