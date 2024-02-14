#pragma once
#include "ds/unit_prod.hpp"
#include "utility/memory_pool.hpp"

template <typename Monoid, bool PERSISTENT, int N = -1, typename F = Unit_Prod<Monoid>>
struct Dynamic_SegTree: F {
    using M = Monoid;
    using X = M::value_type;

    struct Node {
        Node *l, *r;
        X x;
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    i64 L, R;
    Dynamic_SegTree(i64 L, i64 R, F default_prod = {}): F(default_prod), L(L), R(R) {}
    auto default_prod(i64 l, i64 r) { return F::operator()(l, r); }
    np new_node(const X& x) { return pool.new_node({0, 0, x}); }
    np new_node(i64 l, i64 r) {
        return new_node(default_prod(l, r));
    }
    np new_node() { return new_node(L, R); }
    np new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        auto dfs = [&](auto&& dfs, i64 l, i64 r) -> np {
            if (l == r)
                return 0;
            if (l + 1 == r)
                return new_node(a[l]);
            i64 m = (l + r) >> 1;
            np lr = dfs(dfs, l, m), rr = dfs(dfs, m, r);
            X x = M::op(lr->x, rr->x);
            np u = new_node(x);
            u->l = lr, u->r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    X prod(np u, i64 l, i64 r) {
        if (!u || l == r)
            return M::unit();
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, i64 l, i64 r, i64 ql, i64 qr) -> void {
            chkmax(ql, l), chkmin(qr, r);
            if (ql >= qr)
                return;
            if (!u) {
                x = M::op(x, default_prod(ql, qr));
                return;
            }
            if (ql == l && qr == r) {
                x = M::op(x, u->x);
                return;
            }
            i64 m = (l + r) >> 1;
            dfs(dfs, u->l, l, m, ql, qr);
            dfs(dfs, u->r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    X prod_all(np u) { return prod(u, L, R); }
    np set(np u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, i64 l, i64 r) -> np {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = x;
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            X xl = u->l ? u->l->x : default_prod(l, m);
            X xr = u->r ? u->r->x : default_prod(m, r);
            u->x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    np multiply(np u, i64 i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, i64 l, i64 r) -> np {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = M::op(u->x, x);
                return u;
            }
            i64 m = (l + r) >> 1;
            if (i < m)
                u->l = dfs(dfs, u->l, l, m);
            else
                u->r = dfs(dfs, u->r, m, r);
            X xl = u->l ? u->l->x : default_prod(l, m);
            X xr = u->r ? u->r->x : default_prod(m, r);
            u->x = M::op(xl, xr);
            return u;
        };
        return dfs(dfs, u, L, R);
    }
    i64 max_right(np u, auto&& check, i64 ql) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (r <= ql)
                return R;
            if (ql <= l && check(M::op(x, u->x))) {
                x = M::op(x, u->x);
                return R;
            }
            if (l + 1 == r)
                return l;
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    i64 min_left(np u, auto&& check, i64 qr) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, i64 l, i64 r) -> i64 {
            if (!u)
                u = new_node(l, r);
            if (qr <= l)
                return L;
            if (r <= qr && check(M::op(u->x, x))) {
                x = M::op(u->x, x);
                return L;
            }
            if (l + 1 == r)
                return r;
            i64 m = (l + r) >> 1;
            i64 k = dfs(dfs, u->r, m, r);
            if (k != L)
                return k;
            return dfs(dfs, u->l, l, m);
        };
        return dfs(dfs, u, L, R);
    }
    np copy_node(np u) {
        if (u && PERSISTENT)
            return pool.new_node(*u);
        return u;
    }
};