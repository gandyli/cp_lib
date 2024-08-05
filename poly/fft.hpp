#pragma once
#include "template.hpp"

namespace CFFT {
    template <typename T>
    struct Cp {
        T x{}, y{};
        Cp operator+(const Cp& c) const { return {x + c.x, y + c.y}; }
        Cp& operator+=(const Cp& c) { return *this = *this + c; }
        Cp operator-(const Cp& c) const { return {x - c.x, y - c.y}; }
        Cp& operator-=(const Cp& c) { return *this = *this - c; }
        Cp operator*(const Cp& c) const { return {x * c.x - y * c.y, x * c.y + y * c.x}; }
        Cp operator*=(const Cp& c) { return *this = *this * c; }
        Cp operator*(T z) const { return {x * z, y * z}; }
        Cp operator*=(T z) { return *this = *this * z; }
        Cp operator-() const { return {-x, -y}; }
        Cp conj() const { return {x, -y}; }
        Cp rotl() const { return {-y, x}; }
    };
    using C = Cp<f64>;
    vc<C> w;

    void setw(int k) {
        if (len(w) >= (1 << --k))
            return;
        w.resize(1 << k);
        w[0] = {1, 0};
        for (int i = 1; i < (1 << k); i <<= 1) {
            auto arg = std::numbers::pi / (i << 1);
            w[i] = {cos(arg), sin(arg)};
        }
        _for (i, 1, 1 << k)
            w[i] = w[i & (i - 1)] * w[lowbit(i)];
    }
    void fft(vc<C>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            a[0] += std::exchange(a[1], a[0] - a[1]);
            return;
        }
        if (k & 1) {
            int v = 1 << (k - 1);
            _for (j, v)
                a[j] += std::exchange(a[j + v], a[j] - a[j + v]);
        }
        int u = 1 << (k & 1), v = 1 << (k - 2 - (k & 1));
        C imag = w[1];
        while (v) {
            for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
                C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                C t0p2 = t0 + t2, t1p3 = t1 + t3;
                C t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
            }
            _for (jh, 1, u) {
                C ww = w[jh], xx = w[jh << 1], wx = ww * xx;
                for (int j0 = (jh * v) << 2, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                    C t0 = a[j0], t1 = a[j1] * xx, t2 = a[j2] * ww, t3 = a[j3] * wx;
                    C t0p2 = t0 + t2, t1p3 = t1 + t3;
                    C t0m2 = t0 - t2, t1m3 = (t1 - t3) * w[1];
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            u <<= 2, v >>= 2;
        }
    }
    void ifft(vc<C>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            a[0] += std::exchange(a[1], a[0] - a[1]);
            return;
        }
        int u = 1 << (k - 2), v = 1;
        C imag = w[1].conj();
        while (u) {
            for (int j0 = 0, j1 = v, j2 = j1 + v, j3 = j2 + v; j0 < v; j0++, j1++, j2++, j3++) {
                C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                C t0p1 = t0 + t1, t2p3 = t2 + t3;
                C t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
            }
            _for (jh, 1, u) {
                C ww = w[jh].conj(), xx = w[jh << 1].conj(), yy = w[jh << 1 | 1].conj();
                for (int j0 = (jh * v) << 2, j1 = j0 + v, j2 = j1 + v, j3 = j2 + v, je = j1; j0 < je; j0++, j1++, j2++, j3++) {
                    C t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    C t0p1 = t0 + t1, t2p3 = t2 + t3;
                    C t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j1] = t0m1 + t2m3, a[j3] = (t0m1 - t2m3) * ww;
                }
            }
            u >>= 2, v <<= 2;
        }
        if (k & 1) {
            u = 1 << (k - 1);
            _for (j, u)
                a[j] += std::exchange(a[j + u], a[j] - a[j + u]);
        }
    }
} // namespace CFFT