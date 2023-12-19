#pragma once
#include "monoid/add.hpp"
#include "monoid/min.hpp"
#include "monoid/monoid_combine.hpp"

template <typename T>
using Monoid_AddMin = Monoid_Combine<Monoid_Add<T>, Monoid_Min<T>>;