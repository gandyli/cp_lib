#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A"

#include "all.hpp"
#include "graph/utility.hpp"

int main() {
    dR(int, n);
    Graph<int> g(n);
    g.read_tree(true, 0);
    writeln(diameter<int>(g).first);
    return 0;
}