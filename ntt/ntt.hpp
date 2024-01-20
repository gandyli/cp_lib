#pragma once
#include "math/primitive_root_constexpr.hpp"
#include "modint/montgomery.hpp"

template <typename mint>
struct NTT {
    static constexpr u32 mod = mint::mod();
    static constexpr mint pr = primitive_root_constexpr(mod);
    static constexpr int lvl = __builtin_ctz(mod - 1);
    mint dw[lvl], dy[lvl];

    constexpr void setwy(int k) {
        mint w[lvl], y[lvl];
        w[k - 1] = power(pr, (mod - 1) / (1 << k));
        y[k - 1] = w[k - 1].inv();
        _for_r (i, 1, k - 1)
            w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
        dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
        _for (i, 3, k) {
            dw[i] = dw[i - 1] * y[i - 2] * w[i];
            dy[i] = dy[i - 1] * w[i - 2] * y[i];
        }
    }

    constexpr NTT() { setwy(lvl); }

    void fft4(vc<mint>& a, int k) {
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
        int u = 1 << (2 + (k & 1));
        int v = 1 << (k - 2 - (k & 1));
        mint one(1);
        mint imag = dw[1];
        while (v) {
            // jh = 0
            {
                int j0 = 0;
                int j1 = v;
                int j2 = j1 + v;
                int j3 = j2 + v;
                for (; j0 < v; j0++, j1++, j2++, j3++) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
                }
            }
            // jh >= 1
            mint ww = one, xx = one * dw[2], wx = one;
            for (int jh = 4; jh < u;) {
                ww = xx * xx, wx = ww * xx;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for (; j0 < je; j0++, j2++) {
                    mint t0 = a[j0], t1 = a[j0 + v] * xx, t2 = a[j2] * ww,
                         t3 = a[j2 + v] * wx;
                    mint t0p2 = t0 + t2, t1p3 = t1 + t3;
                    mint t0m2 = t0 - t2, t1m3 = (t1 - t3) * imag;
                    a[j0] = t0p2 + t1p3, a[j0 + v] = t0p2 - t1p3;
                    a[j2] = t0m2 + t1m3, a[j2 + v] = t0m2 - t1m3;
                }
                xx *= dw[__builtin_ctz(jh += 4)];
            }
            u <<= 2;
            v >>= 2;
        }
    }

    void ifft4(vc<mint>& a, int k) {
        if (len(a) <= 1)
            return;
        if (k == 1) {
            mint a1 = a[1];
            a[1] = a[0] - a[1];
            a[0] = a[0] + a1;
            return;
        }
        int u = 1 << (k - 2);
        int v = 1;
        mint one(1);
        mint imag = dy[1];
        while (u) {
            // jh = 0
            {
                int j0 = 0;
                int j1 = v;
                int j2 = v + v;
                int j3 = j2 + v;
                for (; j0 < v; j0++, j1++, j2++, j3++) {
                    mint t0 = a[j0], t1 = a[j1], t2 = a[j2], t3 = a[j3];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = t0 - t1, t2m3 = (t2 - t3) * imag;
                    a[j0] = t0p1 + t2p3, a[j2] = t0p1 - t2p3;
                    a[j1] = t0m1 + t2m3, a[j3] = t0m1 - t2m3;
                }
            }
            // jh >= 1
            mint ww = one, xx = one * dy[2], yy = one;
            u <<= 2;
            for (int jh = 4; jh < u;) {
                ww = xx * xx, yy = xx * imag;
                int j0 = jh * v;
                int je = j0 + v;
                int j2 = je + v;
                for (; j0 < je; j0++, j2++) {
                    mint t0 = a[j0], t1 = a[j0 + v], t2 = a[j2], t3 = a[j2 + v];
                    mint t0p1 = t0 + t1, t2p3 = t2 + t3;
                    mint t0m1 = (t0 - t1) * xx, t2m3 = (t2 - t3) * yy;
                    a[j0] = t0p1 + t2p3, a[j2] = (t0p1 - t2p3) * ww;
                    a[j0 + v] = t0m1 + t2m3, a[j2 + v] = (t0m1 - t2m3) * ww;
                }
                xx *= dy[__builtin_ctz(jh += 4)];
            }
            u >>= 4;
            v <<= 2;
        }
        if (k & 1) {
            u = 1 << (k - 1);
            _for (j, u)
                a[j] += std::exchange(a[j + u], a[j] - a[j + u]);
        }
    }

    void ntt(vc<mint>& a) {
        if (len(a) <= 1)
            return;
        fft4(a, __builtin_ctz(len(a)));
    }
    void intt(vc<mint>& a) {
        if (len(a) <= 1)
            return;
        ifft4(a, __builtin_ctz(len(a)));
        mint iv = mint(len(a)).inv();
        foreach (x, a)
            x *= iv;
    }

    vc<mint> multiply(const vc<mint>& a, const vc<mint>& b) {
        if (a.empty() && b.empty())
            return {};
        int l = len(a) + len(b) - 1;
        if (min(len(a), len(b)) <= 40) {
            vc<mint> s(l);
            _for (i, len(a))
                _for (j, len(b))
                    s[i + j] += a[i] * b[j];
            return s;
        }
        int k = 2, M = 4;
        while (M < l)
            M <<= 1, k++;
        vc<mint> s(M);
        _for (i, len(a))
            s[i] = a[i];
        fft4(s, k);
        if (a == b)
            _for (i, M)
                s[i] *= s[i];
        else {
            vc<mint> t(M);
            _for (i, len(b))
                t[i] = b[i];
            fft4(t, k);
            _for (i, M)
                s[i] *= t[i];
        }
        ifft4(s, k);
        s.resize(l);
        mint invm = mint(M).inv();
        _for (i, l)
            s[i] *= invm;
        return s;
    }
    void ntt_doubling(vc<mint>& a) {
        int M = len(a);
        auto b = a;
        intt(b);
        mint r = 1, zeta = power(pr, (mod - 1) / (M << 1));
        _for (i, M)
            b[i] *= r, r *= zeta;
        ntt(b);
        a.insert(a.end(), all(b));
    }
};