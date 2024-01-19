#pragma once
#include "template.hpp"

namespace RadixSortImpl {
    constexpr int b = 8;
    constexpr int powb = 1 << b;
    constexpr int mask = powb - 1;

    int c0[powb], c1[powb], c2[powb], c3[powb];

    template <typename T>
    requires (sizeof(T) == 4 || sizeof(T) == 8)
    void radix_sort(int n, T* p) {
        if (!n)
            return;
        if (n <= 64) {
            sort(p, p + n);
            return;
        }
        static T* tmp = nullptr;
        static int tmp_size = 0;
        if (!tmp || tmp_size < n) {
            delete[] tmp;
            tmp_size = 1;
            while (tmp_size < n)
                tmp_size *= 2;
            tmp = new T[tmp_size];
        }
        memset(c0, 0, sizeof(c0));
        memset(c1, 0, sizeof(c1));
        memset(c2, 0, sizeof(c2));
        memset(c3, 0, sizeof(c3));
        _for (i, n) {
            c0[p[i] & mask]++;
            c1[(p[i] >> b) & mask]++;
            c2[(p[i] >> b * 2) & mask]++;
            c3[(p[i] >> b * 3) & mask]++;
        }
        _for (i, powb - 1) {
            c0[i + 1] += c0[i];
            c1[i + 1] += c1[i];
            c2[i + 1] += c2[i];
            c3[i + 1] += c3[i];
        }
        _for_r (i, n)
            tmp[--c0[p[i] & mask]] = p[i];
        _for_r (i, n)
            p[--c1[tmp[i] >> b & mask]] = tmp[i];
        _for_r (i, n)
            tmp[--c2[p[i] >> b * 2 & mask]] = p[i];
        _for_r (i, n)
            p[--c3[tmp[i] >> b * 3 & mask]] = tmp[i];
        if constexpr (sizeof(T) == 8) {
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[p[i] >> b * 4 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                tmp[--c0[p[i] >> b * 4 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[tmp[i] >> b * 5 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                p[--c0[tmp[i] >> b * 5 & mask]] = tmp[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[p[i] >> b * 6 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                tmp[--c0[p[i] >> b * 6 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[tmp[i] >> b * 7 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                p[--c0[tmp[i] >> b * 7 & mask]] = tmp[i];
        }
        if constexpr (std::is_signed_v<T>) {
            int i = n;
            while (i && p[i - 1] < 0)
                i--;
            rotate(p, p + i, p + n);
        }
    }
    void radix_sort(auto& v) { radix_sort(len(v), v.data()); }
    template <typename T, typename U>
    requires (sizeof(T) == 4 || sizeof(T) == 8)
    void radix_sort_compare_first(int n, std::pair<T, U>* p) {
        if (!n)
            return;
        if (n <= 64) {
            stable_sort(p, p + n, [](auto&& s, auto&& t) { return s.first < t.first; });
            return;
        }
        static std::pair<T, U>* tmp = nullptr;
        static int tmp_size = 0;
        if (!tmp || tmp_size < n) {
            delete[] tmp;
            tmp_size = 1;
            while (tmp_size < n)
                tmp_size *= 2;
            tmp = new std::pair<T, U>[tmp_size];
        }
        memset(c0, 0, sizeof(c0));
        _for (i, n)
            c0[p[i].first & mask]++;
        _for (i, powb - 1)
            c0[i + 1] += c0[i];
        _for_r (i, n)
            tmp[--c0[p[i].first & mask]] = p[i];
        memset(c0, 0, sizeof(c0));
        _for (i, n)
            c0[tmp[i].first >> b & mask]++;
        _for (i, powb - 1)
            c0[i + 1] += c0[i];
        _for_r (i, n)
            p[--c0[tmp[i].first >> b & mask]] = tmp[i];
        memset(c0, 0, sizeof(c0));
        _for (i, n)
            c0[p[i].first >> b * 2 & mask]++;
        _for (i, powb - 1)
            c0[i + 1] += c0[i];
        _for_r (i, n)
            tmp[--c0[p[i].first >> b * 2 & mask]] = p[i];
        memset(c0, 0, sizeof(c0));
        _for (i, n)
            c0[tmp[i].first >> b * 3 & mask]++;
        _for (i, powb - 1)
            c0[i + 1] += c0[i];
        _for_r (i, n)
            p[--c0[tmp[i].first >> b * 3 & mask]] = tmp[i];

        if constexpr (sizeof(T) == 8) {
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[p[i].first >> b * 4 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                tmp[--c0[p[i].first >> b * 4 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[tmp[i].first >> b * 5 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                p[--c0[tmp[i].first >> b * 5 & mask]] = tmp[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[p[i].first >> b * 6 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                tmp[--c0[p[i].first >> b * 6 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            _for (i, n)
                c0[tmp[i].first >> b * 7 & mask]++;
            _for (i, powb - 1)
                c0[i + 1] += c0[i];
            _for_r (i, n)
                p[--c0[tmp[i].first >> b * 7 & mask]] = tmp[i];
        }

        if constexpr (std::is_signed_v<T>) {
            int i = n;
            while (i && p[i - 1].first < 0)
                i--;
            rotate(p, p + i, p + n);
        }
    }
    void radix_sort_compare_first(auto& v) { radix_sort_compare_first(len(v), v.data()); }
} // namespace RadixSortImpl

using RadixSortImpl::radix_sort, RadixSortImpl::radix_sort_compare_first;
