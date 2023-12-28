#pragma once
#include "graph/tree_monoid_base.hpp"
#include "ds/lazy_segtree.hpp"

template <typename TREE, typename ActedMonoid, bool edge = false>
using Lazy_Tree_Monoid = Tree_Monoid_Base<Lazy_SegTree, TREE, ActedMonoid, edge>;