#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C"

#include "all.hpp"
#include "graph/floyd.hpp"

int main() {
    dR(int, n, m);
    Graph<int, true> g(n);
    g.read_graph(m, 0);

    auto dis = floyd<i64>(g);
    _for (i, n)
        if (dis[i][i] < 0) {
            print("NEGATIVE CYCLE");
            return 0;
        }
    _for (i, n) {
        _for (j, n) {
            if (j)
                io.write(' ');
            if (dis[i][j] == inf<i64>)
                io.write("INF");
            else
                io.write(dis[i][j]);
        }
        print();
    }
    return 0;
}