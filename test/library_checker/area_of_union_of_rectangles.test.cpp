#define PROBLEM "https://judge.yosupo.jp/problem/area_of_union_of_rectangles"

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