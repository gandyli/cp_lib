#pragma once
#include "ds/unit_prod.hpp"
#include "utility/memory_pool.hpp"

template <typename Monoid, bool PERSISTENT, int N = -1, typename F = Unit_Prod<Monoid>>
struct Dynamic_SegTree: F {
    using M = Monoid;
    using X = M::value_type;

    struct Node {
        int l, r;
        X x;
    };
    Memory_Pool<Node, N> pool;

    i64 L, R;
    Dynamic_SegTree(i64 L, i64 R, F default_prod = {}): F(default_prod), L(L), R(R) { pool.new_node(); }
    auto default_prod(i64 l, i64 r) { return F::operator()(l, r); }
    int new_node(const X& x) { return pool.new_node_id({0, 0, x}); }
    int new_node(i64 l, i64 r) {
        return new_node(default_prod(l, r));
    }
    int new_node() { return new_node(L, R); }
    int new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        auto dfs = [&](auto&& dfs, i64 l, i64 r) -> int {
            if (l == r)
                return 0;
            if (l + 1 == r)
                return new_node(a[l]);
            i64 m = (l + r) >> 1;
            int lr = dfs(dfs, l, m), rr = dfs(dfs, m, r);
            X x = M::op(pool[lr].x, pool[rr].x);
            int u = new_node(x);
            pool[u].l = lr, pool[u].r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    X prod(int u, i64 l, i64 r) {
        if (!u || l == r)
            return M::unit();
        X x = M::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r, i64 ql, i64 qr) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return;
            if (!u) {
                x = M::op(x, default_prod(ql, qr));
                return;
            }
            if (ql == l && qr == r) {
                x = M::op(x, pool[u].x);
                return;
            }
            i64 m = (l + r) >> 1;
            dfs(dfs, pool[u].l, l, m, ql, qr);
            dfs(dfs, pool[u].r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    X prod_all(int u) { return prod(u, L, R); }
    int set(int u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> int {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                pool[u].x = x;
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m)
                pool[u].l = dfs(dfs, pool[u].l, l, m);
            else
                pool[u].r = dfs(dfs, pool[u].r, m, r);
            X xl = pool[u].l ? pool[pool[u].l].x : default_prod(l, m);
            X xr = pool[u].r ? pool[pool[u].r].x : default_prod(m, r);
            pool[u].x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    int multiply(int u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> int {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                pool[u].x = M::op(pool[u].x, x);
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m)
                pool[u].l = dfs(dfs, pool[u].l, l, m);
            else
                pool[u].r = dfs(dfs, pool[u].r, m, r);
            X xl = pool[u].l ? pool[pool[u].l].x : default_prod(l, m);
            X xr = pool[u].r ? pool[pool[u].r].x : default_prod(m, r);
            pool[u].x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    i64 max_right(int u, auto&& check, i64 ql) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (r <= ql)
                return R;
            if (ql <= l && check(M::op(x, pool[u].x))) {
                x = M::op(x, pool[u].x);
                return R;
            }
            if (l + 1 == r)
                return l;
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, pool[u].l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, pool[u].r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    i64 min_left(int u, auto&& check, i64 qr) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, int u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (qr <= l)
                return L;
            if (r <= qr && check(M::op(pool[u].x, x))) {
                x = M::op(pool[u].x, x);
                return L;
            }
            if (l + 1 == r)
                return r;
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, pool[u].r, m, r);
            if (k != L)
                return k;
            return dfs(dfs, pool[u].l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    int copy_node(int u) {
        if (u && PERSISTENT)
            return pool.new_node_id(pool[u]);
        return u;
    }
};