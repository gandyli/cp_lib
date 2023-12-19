#pragma once
#include "monoid/add.hpp"
#include "monoid/max.hpp"
#include "monoid/monoid_combine.hpp"

template <typename T>
using Monoid_AddMax = Monoid_Combine<Monoid_Add<T>, Monoid_Max<T>>;