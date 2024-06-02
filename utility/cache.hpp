#pragma once
#include "ds/hashmap.hpp"

// https://codeforces.com/blog/entry/124683
template <typename, typename>
struct Cache;
template <typename R, typename... Args, typename F>
struct Cache<R(Args...), F>: F {
    HashMap<std::tuple<Args...>, R> mp;
    explicit Cache(const F& f): F(f) {}
    R operator()(auto&&... args) {
        auto t = std::tuple{FORWARD(args)...};
        if (mp.contains(t))
            return mp[t];
        return mp[t] = F::operator()(*this, FORWARD(args)...);
    }
};
template <typename... Args>
auto use_cache(const auto& f) { return Cache<decltype(f(f, std::declval<Args>()...))(Args...), std::decay_t<decltype(f)>>(f); }