#pragma once
#include "template.hpp"

template <typename T, bool SAME, bool SMALL>
struct Index_Compression;

template <typename T>
struct Index_Compression<T, false, false> {
    vc<T> dat;
    std::pair<vi, int> build(const vc<T>& X) {
        vi I = argsort(X);
        vi a(len(X));
        dat.reserve(len(X));
        foreach (i, I) {
            a[i] = len(dat);
            dat.eb(X[i]);
        }
        return {std::move(a), len(dat)};
    }
    int operator()(T x) const { return LB(dat, x); }
};

template <typename T>
struct Index_Compression<T, true, false> {
    vc<T> dat;
    std::pair<vi, int> build(const vc<T>& X) {
        vi I = argsort(X);
        vi a(len(X));
        dat.reserve(len(X));
        foreach (i, I) {
            if (!dat.empty() && dat.back() == X[i])
                a[i] = len(dat) - 1;
            else {
                a[i] = len(dat);
                dat.eb(X[i]);
            }
        }
        return {std::move(a), len(dat)};
    }
    int operator()(T x) const { return LB(dat, x); }
};

template <typename T>
struct Index_Compression<T, false, true> {
    int L, R;
    vc<T> dat;
    std::pair<vi, int> build(vc<T> X) {
        L = 0, R = -1;
        if (!X.empty())
            L = min(X), R = max(X);
        dat.assign(R - L + 2, 0);
        foreach (x, X)
            dat[x - L + 1]++;
        _for (i, R - L + 1)
            dat[i + 1] += dat[i];
        foreach (x, X)
            x = dat[x - L]++;
        _for_r (i, 1, R - L + 2)
            dat[i] = dat[i - 1];
        dat[0] = 0;
        return {std::move(X), len(X)};
    }
    int operator()(T x) const { return dat[std::clamp(x - L, 0, R - L + 1)]; }
};

template <typename T>
struct Index_Compression<T, true, true> {
    int L, R;
    vc<T> dat;
    std::pair<vi, int> build(vc<T> X) {
        L = 0, R = -1;
        if (!X.empty())
            L = min(X), R = max(X);
        dat.assign(R - L + 2, 0);
        foreach (x, X)
            dat[x - L + 1] = 1;
        _for (i, R - L + 1)
            dat[i + 1] += dat[i];
        foreach (x, X)
            x = dat[x - L];
        return {std::move(X), R - L + 2};
    }
    int operator()(T x) const { return dat[std::clamp<int>(x - L, 0, R - L + 1)]; }
};