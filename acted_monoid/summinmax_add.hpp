#pragma once
#include "acted_monoid/sum_add.hpp"
#include "acted_monoid/min_add.hpp"
#include "acted_monoid/max_add.hpp"
#include "acted_monoid/acted_monoid_combine.hpp"

template <typename T>
using ActedMonoid_SumMinMax_Add = ActedMonoid_Combine<ActedMonoid_Sum_Add<T>, ActedMonoid_Min_Add<T>, ActedMonoid_Max_Add<T>>;