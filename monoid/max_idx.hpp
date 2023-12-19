#pragma once
#include "template.hpp"

template <typename T, bool less = true>
struct Monoid_Max_Idx {
    using value_type = std::pair<T, int>;
    using X = value_type;
    static constexpr X op(const X& a, const X& b) {
        if (a.first > b.first)
            return a;
        if (a.first < b.first)
            return b;
        if constexpr (less)
            return a.second < b.second ? a : b;
        else
            return a.second > b.second ? a : b;
    }
    static constexpr X unit() { return {-inf<T>, -1}; }
    static constexpr bool commute = true;
};