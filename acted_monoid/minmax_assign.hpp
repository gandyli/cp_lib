#pragma once
#include "acted_monoid/min_assign.hpp"
#include "acted_monoid/max_assign.hpp"
#include "acted_monoid/acted_monoid_combine.hpp"

template <typename T>
using ActedMonoid_MinMax_Assign = ActedMonoid_Combine<ActedMonoid_Min_Assign<T>, ActedMonoid_Max_Assign<T>>;