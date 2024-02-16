#pragma once
#include "ds/unit_prod.hpp"
#include "utility/memory_pool.hpp"

template <typename Monoid, bool PERSISTENT, typename T = int, int N = -1, typename F = Unit_Prod<Monoid>>
struct Dynamic_SegTree: F {
    using M = Monoid;
    using X = M::value_type;

    struct Node {
        Node *l, *r;
        X x;
    };
    Memory_Pool<Node, N> pool;
    using np = Node*;

    T L, R;
    Dynamic_SegTree(T L, T R, F default_prod = {}): F(default_prod), L(L), R(R) {}
    auto default_prod(T l, T r) { return F::operator()(l, r); }
    np new_node(const X& x) { return pool.new_node({nullptr, nullptr, x}); }
    np new_node(T l, T r) {
        return new_node(default_prod(l, r));
    }
    np new_node() { return new_node(L, R); }
    np new_node(const vc<X>& a) {
        ASSERT(L == 0 && R == len(a));
        return new_node([&](T i) { return a[i]; });
    }
    np new_node(std::invocable<T> auto&& f) {
        auto dfs = [&](auto&& dfs, T l, T r) -> np {
            if (l == r)
                return nullptr;
            if (l + 1 == r)
                return new_node(f(l));
            T m = (l + r) >> 1;
            np lr = dfs(dfs, l, m), rr = dfs(dfs, m, r);
            X x = M::op(lr->x, rr->x);
            np u = new_node(x);
            u->l = lr, u->r = rr;
            return u;
        };
        return dfs(dfs, L, R);
    }
    X prod(np u, T l, T r) {
        if (!u || l == r)
            return M::unit();
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r, T ql, T qr) -> void {
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
            T m = (l + r) >> 1;
            dfs(dfs, u->l, l, m, ql, qr);
            dfs(dfs, u->r, m, r, ql, qr);
        };
        dfs(dfs, u, L, R, l, r);
        return x;
    }
    X prod_all(np u) { return prod(u, L, R); }
    np set(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> np {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = x;
                return u;
            }
            T m = (l + r) >> 1;
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
    np multiply(np u, T i, const X& x) {
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> np {
            if (!u)
                u = new_node(l, r);
            else
                u = copy_node(u);
            if (l + 1 == r) {
                u->x = M::op(u->x, x);
                return u;
            }
            T m = (l + r) >> 1;
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
    T max_right(np u, auto&& check, T ql) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
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
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->l, l, m);
            if (k != R)
                return k;
            return dfs(dfs, u->r, m, r);
        };
        return dfs(dfs, u, L, R);
    }
    T min_left(np u, auto&& check, T qr) {
        X x = M::unit();
        auto dfs = [&](auto&& dfs, np u, T l, T r) -> T {
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
            T m = (l + r) >> 1;
            T k = dfs(dfs, u->r, m, r);
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