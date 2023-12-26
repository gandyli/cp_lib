#pragma once
#include "acted_monoid/sum_assign.hpp"
#include "acted_monoid/max_assign.hpp"
#include "acted_monoid/acted_monoid_combine.hpp"

template <typename T>
using ActedMonoid_SumMax_Assign = ActedMonoid_Combine<ActedMonoid_Sum_Assign<T>, ActedMonoid_Max_Assign<T>>;