#pragma once
#include "acted_monoid/sum_mul.hpp"
#include "acted_monoid/minmax_mul.hpp"
#include "acted_monoid/acted_monoid_combine.hpp"

template <typename T>
using ActedMonoid_SumMinMax_Mul = ActedMonoid_Combine<ActedMonoid_Sum_Mul<T>, ActedMonoid_MinMax_Mul<T>>;