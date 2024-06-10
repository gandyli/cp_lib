#pragma once
#include "ds/sparse_table.hpp"
#include "monoid/min.hpp"

struct Suffix_Array {
    int n;
    vi sa, isa, LCP;
    Sparse_Table<Monoid_Min<int>> st;

    Suffix_Array(std::string_view s) {
        n = len(s);
        char first = min(s), last = max(s);
        sa = calc_suffix_array(s, first, last);
        calc_lcp(s);
    }
    Suffix_Array(const vi& s) {
        n = len(s);
        sa = calc_suffix_array(s);
        calc_lcp(s);
    }
    void build_lcp() { st.build(LCP); }
    int lcp(int i, int j) {
        ASSERT(!st.st.empty());
        if (i == n || j == n)
            return 0;
        if (i == j)
            return n - i;
        i = isa[i], j = isa[j];
        if (i > j)
            swap(i, j);
        return st.prod(i, j);
    }
    pi lcp_range(int i, int k) { // TODO
        ASSERT(!st.st.empty());
        if (i == n)
            return {0, n};
        i = isa[i];
        auto check = [&](int x) { return x >= k; };
        return {st.min_left(check, i), st.max_right(check, i) + 1};
    }
    int compare(int l0, int r0, int l1, int r1) {
        int n0 = r0 - l0, n1 = r1 - l1;
        int m = lcp(l0, l1);
        if (m == min(n0, n1))
            return n0 - n1;
        return isa[l0 + m] < isa[l1 + m] ? -1 : 1;
    }

    static void induced_sort(const vi& a, int val_range, vi& sa, const vc<bool>& sl,
      const vi& lms_idx) {
        vi l(val_range), r(val_range);
        foreach (c, a) {
            if (c + 1 < val_range)
                l[c + 1]++;
            r[c]++;
        }
        partial_sum(all(l), l.begin());
        partial_sum(all(r), r.begin());
        fill(sa, -1);
        _for_r (i, len(lms_idx))
            sa[--r[a[lms_idx[i]]]] = lms_idx[i];
        foreach (i, sa)
            if (i >= 1 && sl[i - 1])
                sa[l[a[i - 1]]++] = i - 1;
        fill(r, 0);
        foreach (c, a)
            r[c]++;
        partial_sum(all(r), r.begin());
        for (int k = len(sa) - 1, i = sa[k]; k >= 1; i = sa[--k])
            if (i >= 1 && !sl[i - 1])
                sa[--r[a[i - 1]]] = i - 1;
    }
    vi SA_IS(const vi& a, int val_range) {
        const int n = len(a);
        vi sa(n), lms_idx;
        vc<bool> sl(n);
        _for_r (i, n - 1) {
            sl[i] = (a[i] > a[i + 1] || (a[i] == a[i + 1] && sl[i + 1]));
            if (sl[i] && !sl[i + 1])
                lms_idx.eb(i + 1);
        }
        reverse(lms_idx);
        induced_sort(a, val_range, sa, sl, lms_idx);
        vi new_lms_idx(len(lms_idx)), lms_vec(len(lms_idx));
        int k = 0;
        _for (i, n) 
            if (!sl[sa[i]] && sa[i] >= 1 && sl[sa[i] - 1])
                new_lms_idx[k++] = sa[i];
        int cur = 0;
        sa[n - 1] = cur;
        _for (k, 1, len(new_lms_idx)) {
            int i = new_lms_idx[k - 1], j = new_lms_idx[k];
            if (a[i] != a[j]) {
                sa[j] = ++cur;
                continue;
            }
            for (int p = i + 1, q = j + 1;; p++, q++) {
                if (a[p] != a[q]) {
                    sa[j] = ++cur;
                    break;
                }
                if ((!sl[p] && sl[p - 1]) || (!sl[q] && sl[q - 1])) {
                    sa[j] = (sl[p] || !sl[p - 1] || sl[q] || !sl[q - 1] ? ++cur : cur);
                    break;
                }
            }
        }
        _for (i, len(lms_idx))
            lms_vec[i] = sa[lms_idx[i]];
        if (cur + 1 < len(lms_idx)) {
            auto lms_sa = SA_IS(lms_vec, cur + 1);
            _for (i, len(lms_idx))
                new_lms_idx[i] = lms_idx[lms_sa[i]];
        }
        induced_sort(a, val_range, sa, sl, new_lms_idx);
        return sa;
    }
    vc<int> calc_suffix_array(std::string_view s, char first, char last) {
        vi a(len(s) + 1);
        _for (i, len(s))
            a[i] = s[i] - first + 1;
        auto ret = SA_IS(a, last - first + 2);
        ret.erase(ret.begin());
        return ret;
    }
    vc<int> calc_suffix_array(const vi& s) {
        vi ss(s);
        UNIQUE(ss);
        vi a(len(s) + 1);
        _for (i, len(s))
            a[i] = LB(ss, s[i]) + 1;
        auto ret = SA_IS(a, len(ss) + 2);
        ret.erase(ret.begin());
        return ret;
    }
    void calc_lcp(auto&& s) {
        int n = len(s), k = 0;
        isa.resize(n);
        LCP.resize(n);
        _for (i, n)
            isa[sa[i]] = i;
        _for (i, n) {
            if (isa[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[isa[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k])
                k++;
            LCP[isa[i]] = k;
            if (k)
                k--;
        }
        LCP.pop_back();
    }
};