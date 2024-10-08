#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A"

#include "all.hpp"
#include "graph/utility.hpp"

int main() {
    dR(int, n);
    auto g = read_tree<int>(n, 0);
    print(diameter<int>(g).diam);
    return 0;
}