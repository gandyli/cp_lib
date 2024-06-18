#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_4_A"

#include "all.hpp"
#include "ds/rectangle_union.hpp"

int main() {
    dR(int, n);
    Rectangle_Union ru;
    _for (n) {
        dR(int, xl, yl, xr, yr);
        ru.add(xl, xr, yl, yr);
    }
    print(ru.calc());
    return 0;
}