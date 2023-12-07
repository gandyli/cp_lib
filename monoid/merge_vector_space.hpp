#pragma once
#include "linalg/vector_space.hpp"

template <typename T>
struct Merge_Vector_Space {
    using value_type = Vector_Space<T>;
    using X = value_type;
    static X op(const X& a, const X& b) { return X::merge(a, b); }
    static X unit() { return {}; }
    static constexpr bool commute = true;
};