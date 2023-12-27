#pragma once
#include "graph/tree_monoid_base.hpp"
#include "ds/dual_segtree.hpp"

template <typename TREE, typename Monoid, bool edge = false>
using Dual_Tree_Monoid = Tree_Monoid_Base<Dual_SegTree, TREE, Monoid, edge>;