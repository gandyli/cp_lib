#pragma once
#include "io.hpp"

template <typename T>
struct Edge {
    int from, to;
    T cost;
    int id;
    operator int() const { return to; }
};
template <>
struct Edge<void> {
    int from, to, id;
    operator int() const { return to; }
};

template <typename T = void, bool directed = false>
struct Graph {
    static constexpr bool is_directed() { return directed; }
    static constexpr bool is_weighted() { return !std::is_same_v<T, void>; }

    using cost_type = std::conditional_t<is_weighted(), T, int>;
    using edge_type = Edge<T>;

    int n{}, m{};
    vc<edge_type> edges;
    vi indptr;
    vc<edge_type> csr_edges;

    vi _deg, _indeg, _outdeg;
    struct OutgoingEdges {
        OutgoingEdges(const Graph* G, int l, int r): G(G), l(l), r(r) {}
        const edge_type* begin() const { return G->csr_edges.data() + l; }
        const edge_type* end() const { return G->csr_edges.data() + r; }

    private:
        const Graph* G;
        int l, r;
    };
    Graph() = default;
    Graph(int n): n(n) {}
    Graph(int n, int m): n(n) { edges.reserve(m); }
    void add(int from, int to, int id = -1) requires (!is_weighted())
    {
        if (id == -1)
            id = m;
        edges.eb(from, to, id), m++;
    }
    void add(int from, int to, cost_type cost, int id = -1) requires (is_weighted())
    {
        if (id == -1)
            id = m;
        edges.eb(from, to, cost, id), m++;
    }
    void build() {
        indptr.assign(n + 1, 0);
        foreach (e, edges) {
            indptr[e.from + 1]++;
            if constexpr (!directed)
                indptr[e.to + 1]++;
        }
        _for (i, n)
            indptr[i + 1] += indptr[i];
        auto counter = indptr;
        csr_edges.resize(indptr.back() + 1);
        foreach (e, edges) {
            csr_edges[counter[e.from]++] = e;
            if constexpr (!directed) {
                swap(e.from, e.to);
                csr_edges[counter[e.from]++] = e;
                swap(e.from, e.to);
            }
        }
    }
    OutgoingEdges operator[](int u) const { return {this, indptr[u], indptr[u + 1]}; }
    const vi& Deg() {
        if (_deg.empty())
            calc_deg();
        return _deg;
    }
    const vi& In_deg() {
        if (_indeg.empty())
            calc_deg_inout();
        return _indeg;
    }
    const vi& Out_deg() {
        if (_outdeg.empty())
            calc_deg_inout();
        return _outdeg;
    }
    int deg(int u) { return Deg()[u]; }
    int in_deg(int u) { return In_deg()[u]; }
    int out_deg(int u) { return Out_deg()[u]; }
    Graph reverse() const requires directed
    {
        Graph g0(n);
        foreach (e, edges) {
            if constexpr (is_weighted())
                g0.add(e.to, e.from, e.cost, e.id);
            else
                g0.add(e.to, e.from, e.id);
        }
        g0.build();
        return g0;
    }

    void write(IO& io) const requires (!is_weighted())
    {
        io.print("from to id");
        _for (i, n)
            foreach (e, (*this)[i])
                io.print(e.from, e.to, e.id);
    }
    void write(IO& io) const requires (is_weighted())
    {
        io.print("from to cost id");
        _for (i, n)
            foreach (e, (*this)[i])
                io.print(e.from, e.to, e.cost, e.id);
    }

private:
    void calc_deg() {
        _deg.resize(n);
        foreach (e, edges)
            _deg[e.from]++, _deg[e.to]++;
    }
    void calc_deg_inout() {
        _indeg.resize(n);
        _outdeg.resize(n);
        foreach (e, edges)
            _indeg[e.to]++, _outdeg[e.from]++;
    }
};
template <typename G>
concept DirectedGraph = G::is_directed();
template <typename G>
concept UndirectedGraph = !DirectedGraph<G>;
template <typename G>
concept WeightedGraph = G::is_weighted();

template <typename T = void, bool directed = false>
auto read_graph(int n, int m, int off = 1) {
    Graph<T, directed> g(n, m);
    _for (m) {
        dR(int, a, b), a -= off, b -= off;
        if constexpr (g.is_weighted()) {
            dR(T, c);
            g.add(a, b, c);
        }
        else
            g.add(a, b);
    }
    g.build();
    return g;
}
template <typename T = void, bool directed = false>
auto read_tree(int n, int off = 1) { return read_graph<T, directed>(n, n - 1, off); }