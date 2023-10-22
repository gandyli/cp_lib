#include "factorize.hpp"
#include "prime/lpf_table.hpp"

template <typename R>
R lcm(const vi& a) {
    if (a.empty())
        return 1;
    std::map<int, int> mp;
    foreach (x, a)
        for (auto&& [p, c]: factorize_pair(u32(x)))
            chkmax(mp[p], c);
    R x = 1;
    for (auto&& [p, c]: mp)
        x *= power(R(p), c);
    return x;
}
template <typename R>
R lcm(const vi& a, const vi& lpf) {
    if (a.empty())
        return 1;
    std::map<int, int> mp;
    foreach (x, a)
        for (auto&& [p, c]: factorize_pair(x, lpf))
            chkmax(mp[p], c);
    R x = 1;
    for (auto&& [p, c]: mp)
        x *= power(R(p), c);
    return x;
}