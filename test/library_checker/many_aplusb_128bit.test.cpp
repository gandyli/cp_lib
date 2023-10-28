#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"

#include "all.hpp"

int main() {
    multipleTests([&]{
        dR(i128, a, b);
        print(a + b);
    });
    return 0;
}