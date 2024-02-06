#pragma once
#include "utility/pbds.hpp"
#include "random/hash.hpp"

// https://codeforces.com/blog/entry/124683
template <typename, typename>
struct Cache;
template <typename R, typename... Args, typename F>
struct Cache<R(Args...), F>: public F {
    pbds::unordered_map<std::tuple<Args...>, R, hash> mp;
    explicit Cache(const F& f, int): F(f) {}
    R operator()(auto&&... args) {
        auto t = std::tuple{FORWARD(args)...};
        auto it = mp.find(t);
        if (it != mp.end())
            return it->second;
        return mp[t] = F::operator()(*this, FORWARD(args)...);
    }
};
template <typename... Args>
auto use_cache(const auto& f) { return Cache<decltype(f(f, std::declval<Args>()...))(Args...), std::decay_t<decltype(f)>>(f, {}); }