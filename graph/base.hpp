#pragma once
#include "../io.hpp"

template <typename T>
struct Edge {
    int from, to;
    T cost;
    int id;
    operator int() const { return to; }
};

template <typename T = int, bool directed = false>
struct Graph {
    using cost_type = T;
    using edge_type = Edge<cost_type>;

    int n{}, m{};
    Vec<edge_type> edges;
    vi indptr;
    Vec<edge_type> csr_edges;

    vi _deg, _indeg, _outdeg;
    class OutgoingEdges {
    public:
        OutgoingEdges(const Graph* G, int l, int r): G(G), l(l), r(r) {}
        const edge_type* begin() const { return l == r ? nullptr : &G->csr_edges[l]; }
        const edge_type* end() const { return l == r ? nullptr : &G->csr_edges[r]; }

    private:
        const Graph* G;
        int l, r;
    };
    static constexpr bool is_directed() { return directed; }
    Graph() = default;
    Graph(int n): n(n) {}
    Graph(int n, int m): n(n) { edges.reserve(m); }
    void add(int from, int to, cost_type cost = 1, int id = -1) {
        if (id == -1)
            id = m;
        edges.eb(from, to, cost, id), m++;
    }
    void read_tree(bool w = false, int off = 1) { read_graph(n - 1, w, off); }
    void read_graph(int m, bool w = false, int off = 1) {
        edges.reserve(m);
        _for (m) {
            dR(int, a, b), a -= off, b -= off;
            if (!w)
                add(a, b);
            else {
                dR(cost_type, c);
                add(a, b, c);
            }
        }
        build();
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
            if constexpr (!directed)
                csr_edges[counter[e.to]++] = {e.to, e.from, e.cost, e.id};
        }
    }
    OutgoingEdges operator[](int u) const {
        return {this, indptr[u], indptr[u + 1]};
    }
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
    Graph reverse() const {
        static_assert(directed);
        Graph g0(n);
        foreach (e, edges)
            g0.add(e.to, e.from, e.cost, e.id);
        g0.build();
        return g0;
    }

    void write(IO& io) const {
        io.writeln("from to cost id");
        _for (i, n)
            foreach (e, (*this)[i])
                io.writeln(e.from, ' ', e.to, ' ', e.cost, ' ', e.id);
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
concept UndirectedGraph = !G::is_directed();