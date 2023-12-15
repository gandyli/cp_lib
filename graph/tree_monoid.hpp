#pragma once
#include "graph/tree_monoid_base.hpp"
#include "ds/segtree.hpp"

template <typename TREE, typename Monoid, bool edge = false>
struct Tree_Monoid: Tree_Monoid_Base<SegTree, TREE, Monoid, edge> {
    using M = Monoid;
    using X = M::value_type;
    using Tree_Monoid_Base<SegTree, TREE, Monoid, edge>::Tree_Monoid_Base;

    void set(int i, const X& x) {
        if constexpr (edge)
            i = this->tree.e_to_v(i);
        i = this->tree.lid[i];
        this->ds.set(i, x);
        if constexpr(!M::commute)
            this->rds.set(i, x);
    }
    void multiply(int i, const X& x) {
        if constexpr (edge)
            i = this->tree.e_to_v(i);
        i = this->tree.lid[i];
        this->ds.multiply(i, x);
        if constexpr(!M::commute)
            this->rds.multiply(i, x);
    }
};