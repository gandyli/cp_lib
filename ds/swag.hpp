#pragma once
#include "template.hpp"

template <typename Monoid>
struct SWAG {
    using MX = Monoid;
    using X = MX::value_type;

    int sz{};
    vc<X> dat, cum_l{{MX::unit()}};
    X cum_r{MX::unit()};

    int size() const { return sz; }
    void push(X x) {
        sz++;
        cum_r = MX::op(cum_r, x);
        dat.eb(std::move(x));
    }
    void emplace(auto&&... args) { push(X(FORWARD(args)...)); }
    void pop() {
        ASSERT(sz);
        sz--;
        cum_l.pop_back();
        if (cum_l.empty())
            rebuild();
    }
    void rebuild() {
        cum_l = {MX::unit()};
        cum_r = MX::unit();
        _for_r (i, 1, len(dat))
            cum_l.eb(MX::op(dat[i], cum_l.back()));
        dat.clear();
    }
    X lprod() const { return cum_l.back(); }
    X rprod() const { return cum_r; }
    X prod() const { return MX::op(lprod(), rprod()); }
};

template <typename Monoid>
struct SWAG_deque {
    using MX = Monoid;
    using X = MX::value_type;

    int sz{};
    vc<X> dat_l, dat_r;
    vc<X> cum_l{{MX::unit()}}, cum_r{{MX::unit()}};

    int size() const { return sz; }
    void push_back(X x) {
        sz++;
        cum_r.eb(MX::op(cum_r.back(), x));
        dat_r.eb(std::move(x));
    }
    void emplace_back(auto&&... args) { push_back(X(FORWARD(args)...)); }
    void push_front(X x) {
        sz++;
        cum_l.eb(MX::op(x, cum_l.back()));
        dat_l.eb(std::move(x));
    }
    void emplace_front(auto&&... args) { push_front(X(FORWARD(args)...)); }
    void pop_back() {
        ASSERT(sz);
        if (dat_r.empty()) {
            int t = len(dat_l) / 2;
            dat_r = {t + rall(dat_l)};
            dat_l.erase(all(dat_l) - t);
            rebuild();
        }
        sz--;
        dat_r.pop_back();
        cum_r.pop_back();
    }
    void pop_front() {
        ASSERT(sz);
        if (dat_l.empty()) {
            int t = len(dat_r) / 2;
            dat_l = {t + rall(dat_r)};
            dat_r.erase(all(dat_r) - t);
            rebuild();
        }
        sz--;
        dat_l.pop_back();
        cum_l.pop_back();
    }
    void rebuild() {
        cum_l.resize(len(dat_l) + 1);
        cum_r.resize(len(dat_r) + 1);
        cum_l[0] = MX::unit();
        _for (i, len(dat_l))
            cum_l[i + 1] = MX::op(dat_l[i], cum_l[i]);
        cum_r[0] = MX::unit();
        _for (i, len(dat_r))
            cum_r[i + 1] = MX::op(cum_r[i], dat_r[i]);
    }
    X lprod() const { return cum_l.back(); }
    X rprod() const { return cum_r.back(); }
    X prod() const { return MX::op(lprod(), rprod()); }
};