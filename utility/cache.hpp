#pragma once
#include "utility/pbds.hpp"
#include "random/hash.hpp"

// https://codeforces.com/blog/entry/124683
template <typename, typename>
struct Cache;
template <typename R, typename... Args, typename F>
struct Cache<R(Args...), F> {
    F f;
    pbds::unordered_map<std::tuple<Args...>, R, hash> mp;
    explicit Cache(auto&& f, int): f(FORWARD(f)) {}
    R operator()(auto&&... args) {
        auto t = std::tuple{FORWARD(args)...};
        auto it = mp.find(t);
        if (it != mp.end())
            return it->second;
        return mp[t] = f(*this, FORWARD(args)...);
    }
};
template <typename... Args>
auto use_cache(auto&& f) { return Cache<decltype(f(f, std::declval<Args>()...))(Args...), decltype(f)>(FORWARD(f), {}); }