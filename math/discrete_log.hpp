#pragma once
#include "acted_set/from_monoid.hpp"
#include "ds/hashset.hpp"

template <typename ActedSet, typename A = ActedSet::A, typename S = ActedSet::S>
std::optional<S> discrete_log_acted(A x, S s, S t, auto&& H, i64 L, i64 R) {
    using M = ActedSet::Monoid_A;
    using X = M::value_type;

    if (L >= R)
        return std::nullopt;

    auto xpow = [&](auto n) {
        X p = x;
        X res = M::unit();
        while (n) {
            if (n & 1)
                res = M::op(res, p);
            p = M::op(p, p);
            n >>= 1;
        }
        return res;
    };

    auto Ht = H(t);
    s = ActedSet::act(s, xpow(L));
    u64 LIM = R - L;
    i64 K = sqrt(LIM) + 1;
    HashSet<decltype(Ht)> hs(K);
    _for (i, K) {
        t = ActedSet::act(t, x);
        hs.insert(H(t));
    }
    X y = xpow(K);
    bool ok = true;
    _for (i, K + 1) {
        S s1 = ActedSet::act(s, y);
        if (hs.contains(H(s1))) {
            _for (j, K) {
                if (H(s) == Ht) {
                    auto ans = i * K + j + L;
                    if (ans < R)
                        return ans;
                    return std::nullopt;
                }
                s = ActedSet::act(s, x);
            }
            if (!ok)
                return std::nullopt;
            ok = false;
        }
        s = s1;
    }
    return std::nullopt;
}
template <typename Monoid, typename X = Monoid::value_type>
std::optional<X> discrete_log_monoid(X a, X b, auto&& H, i64 L, i64 R) {
    return discrete_log_acted<ActedSet_From_Monoid<Monoid>>(a, Monoid::unit(), b, H, L, R);
}