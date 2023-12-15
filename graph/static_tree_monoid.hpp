#pragma once
#include "graph/tree_monoid_base.hpp"
#include "ds/disjoint_sparse_table.hpp"

template <typename TREE, typename Monoid, bool edge = false>
using Static_Tree_Monoid = Tree_Monoid_Base<Disjoint_Sparse_Table, TREE, Monoid, edge>;