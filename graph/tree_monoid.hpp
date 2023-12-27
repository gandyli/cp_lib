#pragma once
#include "graph/tree_monoid_base.hpp"
#include "ds/segtree.hpp"

template <typename TREE, typename Monoid, bool edge = false>
using Tree_Monoid = Tree_Monoid_Base<SegTree, TREE, Monoid, edge>;