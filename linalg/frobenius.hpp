#pragma once
#include "linalg/matrix_mul.hpp"
#include "random/base.hpp"

template <typename mint>
struct Frobenius {
    struct Basis {
        vvc<mint> a, b, c;
        int n;
        Basis() = default;
        Basis(int n): a(n, vc<mint>(n)), c(n, vc<mint>(n)), n(n) {}
        int size() const { return len(b); }

        vc<mint> ps;
        bool add(const vc<mint>& us) {
            auto vs{us};
            fill(ps, 0);
            _for (i, n)
                if (vs[i] != 0) {
                    if (c[i][i] == 0) {
                        const mint t = vs[i].inv();
                        _for (j, size())
                            a[i][j] = -t * ps[j];
                        a[i][size()] = t;
                        _for (j, i, n)
                            c[i][j] = vs[j] * t;
                        fill(ps, 0);
                        ps.eb(1);
                        b.eb(us);
                        return true;
                    }
                    const mint t = vs[i];
                    _for (j, size())
                        ps[j] += t * a[i][j];
                    _for (j, i, n)
                        vs[j] -= t * c[i][j];
                }
            return false;
        }
        void reduce() {
            _for_r (i, n)
                _for (j, i + 1, n)
                    _for (k, n)
                        a[i][k] -= c[i][j] * a[j][k];
        }
    };
    int n, m;
    Basis basis;
    vi pt;
    vvc<mint> pss;
    Frobenius(const vvc<mint>& a): n(len(a)) {
        ASSERT(n == len(a[0]));
    Loop:
        basis = Basis(n);
        m = 0;
        pt = {0};
        pss.clear();
        while (len(basis) < n) {
            vc<mint> us(n);
            generate(us, rnd64);
            auto tester = basis;
            for (auto vs{us}; tester.add(vs); vs = mul(a, vs)) {}
            const int di = len(tester) - pt[m];
            pss.eb(tester.ps.end() - di, tester.ps.end());
            _for (i, m) {
                const int d = pt[i + 1] - pt[i];
                if (d < di)
                    goto Loop;
                auto rs = tester.ps.begin() + pt[i];
                _for_r (l, d - di)
                    _for (k, di)
                        rs[l + k] += pss[m][k] * rs[l + di];
                _for (j, di)
                    if (rs[j] != 0)
                        goto Loop;
                _for (l, d - di)
                    _for (j, n)
                        us[j] -= rs[l + di] * basis.b[pt[i] + l][j];
            }
            _for (di) {
                basis.add(us);
                us = mul(a, us);
            }
            m++;
            pt.eb(len(basis));
        }
        basis.reduce();
    }
    vvc<mint> operator()(auto e) const { return matrix_mul(left(), matrix_mul(middle(e), right())); }

private:
    vc<mint> mul(const vvc<mint>& a, const vc<mint>& b) const {
        vc<mint> c(n);
        _for (i, n)
            _for (j, n)
                c[i] += a[i][j] * b[j];
        return c;
    }
    static vc<mint> power(const vc<mint>& fs, auto e) {
        const int n = len(fs);
        if (!e) {
            vc<mint> gs(n);
            if (n)
                gs[0] = 1;
            return gs;
        }
        auto gs = power(fs, e >> 1);
        vc<mint> hs(2 * n - 1);
        _for (i, n)
            _for (j, n)
                hs[i + j] += gs[i] * gs[j];
        _for_r (i, n - 1)
            _for (j, n)
                hs[i + j] += fs[j] * hs[i + n];
        if (e & 1) {
            hs.insert(hs.begin(), 0);
            _for (j, n)
                hs[j] += fs[j] * hs[n];
        }
        hs.resize(n);
        return hs;
    }
    vvc<mint> left() const { return transpose(basis.b); }
    vvc<mint> right() const { return transpose(basis.a); }
    vvc<mint> middle(auto e) const {
        VEC(mint, frob, n, n);
        _for (i, m) {
            const int di = pt[i + 1] - pt[i];
            auto fs0 = power(pss[i], e);
            fs0.resize(2 * di);
            auto fs = begin(rotate(fs0, fs0.begin() + di));
            _for (j, di) {
                _for (k, di)
                    frob[pt[i] + k][pt[i] + j] = fs[k];
                --fs;
                _for (k, di)
                    fs[k] += pss[i][k] * fs[di];
            }
        }
        return frob;
    }
};