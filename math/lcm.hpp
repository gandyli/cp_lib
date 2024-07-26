#pragma once
#include "math/factorize.hpp"
#include "ds/hashmap.hpp"

template <typename R>
R lcm(const vi& a) {
    if (a.empty())
        return 1;
    HashMap<int, int> mp;
    foreach (x, a)
        foreach (p, c, factorize_pair<u32>(x))
            chkmax(mp[p], c);
    R x = 1;
    foreach (p, c, mp)
        x *= power<R>(p, c);
    return x;
}
template <typename R>
R lcm(const vi& a, const vi& lpf) {
    if (a.empty())
        return 1;
    HashMap<int, int> mp;
    foreach (x, a)
        foreach (p, c, factorize_pair(x, lpf))
            chkmax(mp[p], c);
    R x = 1;
    foreach (p, c, mp)
        x *= power<R>(p, c);
    return x;
}