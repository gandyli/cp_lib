#pragma once
#include "poly/convolution.hpp"
#include "monoid/mul.hpp"
#include "ds/swag.hpp"
#include "modint/binomial.hpp"

template <Modint mint>
mint lagrange_interpolate_iota(const vc<mint>& f, mint k) {
    int n = len(f);
    if (k.val() < n)
        return f[k.val()];
    auto a{f};
    _for (i, n) {
        a[i] *= fac_inv<mint>(i) * fac_inv<mint>(n - 1 - i);
        if ((n - 1 - i) & 1)
            a[i] = -a[i];
    }
    vc<mint> lp(n + 1), rp(n + 1);
    lp[0] = rp[n] = 1;
    _for (i, n)
        lp[i + 1] = lp[i] * (k - i);
    _for_r (i, n)
        rp[i] = rp[i + 1] * (k - i);
    mint ans = 0;
    _for (i, n)
        ans += a[i] * lp[i] * rp[i + 1];
    return ans;
}
template <Modint mint>
mint lagrange_interpolate_iota(const vc<mint>& X, const vc<mint>& f, mint k) {
    int n = len(f);
    mint ans = 0;
    _for (i, n) {
        mint s1 = 1, s2 = 1;
        _for (j, n) {
            if (i == j)
                continue;
            s1 *= k - X[j];
            s2 *= X[i] - X[j];
        }
        ans += f[i] * s1 / s2;
    }
    return ans;
}
// [x, x + m)
template <Modint mint>
vc<mint> lagrange_interpolate_iota(const vc<mint>& f, mint x, int m) {
    if (m <= 60) {
        vc<mint> ans(m);
        _for (i, m)
            ans[i] = lagrange_interpolate_iota(f, x + i);
        return ans;
    }
    int n = len(f);
    auto a{f};
    _for (i, n) {
        a[i] *= fac_inv<mint>(i) * fac_inv<mint>(n - 1 - i);
        if ((n - 1 - i) & 1)
            a[i] = -a[i];
    }
    vc<mint> b(n + m - 1);
    _for (i, n + m - 1)
        b[i] = 1 / (x + (i - n + 1));
    a = convolution(a, b);
    SWAG<Monoid_Mul<mint>> swag;
    vc<mint> ans(m);
    int l = 0, r = 0;
    _for (i, m) {
        while (l < i) {
            swag.pop();
            l++;
        }
        while (r - l < n) {
            swag.push(x + (r - n + 1));
            r++;
        }
        auto coef = swag.prod();
        if (coef == 0)
            ans[i] = f[(x + i).val()];
        else
            ans[i] = coef * a[i + n - 1];
    }
    return ans;
}