#pragma once
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace pbds {
    using namespace __gnu_pbds;
    template <typename K, typename V, typename H>
    using unordered_map = gp_hash_table<K, V, H, std::equal_to<>, direct_mask_range_hashing<>, linear_probe_fn<>,
      hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>>;
    template <typename K, typename H>
    using unordered_set = unordered_map<K, null_type, H>;
    template <typename K, typename V, typename Compare = std::less<>>
    using map = tree<K, V, Compare, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename K, typename V, typename Compare = std::less_equal<>>
    using multimap = tree<K, V, Compare, rb_tree_tag, tree_order_statistics_node_update>;
    template <typename T, typename Compare = std::less<>>
    using set = map<T, null_type, Compare>;
    template <typename T, typename Compare = std::less_equal<>>
    using multiset = multimap<T, null_type, Compare>;
} // namespace pbds