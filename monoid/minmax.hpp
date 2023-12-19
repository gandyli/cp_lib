#pragma once
#include "monoid/min.hpp"
#include "monoid/max.hpp"
#include "monoid/monoid_combine.hpp"

template <typename T>
using Monoid_MinMax = Monoid_Combine<Monoid_Min<T>, Monoid_Max<T>>;