#pragma once

#include "template.hpp"

namespace RadixSortImpl {
    constexpr int b = 8;
    constexpr int powb = 1 << b;
    constexpr int mask = powb - 1;

    int c0[powb], c1[powb], c2[powb], c3[powb];

    template <typename T>
    void radix_sort(int n, T* p) {
        static_assert(sizeof(T) == 4 || sizeof(T) == 8, "");
        if (!n)
            return;
        if (n <= 64)
            return std::sort(p, p + n);
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
        for (int i = 0; i < n; i++) {
            c0[p[i] & mask]++;
            c1[(p[i] >> b) & mask]++;
            c2[(p[i] >> b * 2) & mask]++;
            c3[(p[i] >> b * 3) & mask]++;
        }
        for (int i = 0; i < powb - 1; i++) {
            c0[i + 1] += c0[i];
            c1[i + 1] += c1[i];
            c2[i + 1] += c2[i];
            c3[i + 1] += c3[i];
        }
        for (int i = n; i--;)
            tmp[--c0[p[i] & mask]] = p[i];
        for (int i = n; i--;)
            p[--c1[tmp[i] >> b & mask]] = tmp[i];
        for (int i = n; i--;)
            tmp[--c2[p[i] >> b * 2 & mask]] = p[i];
        for (int i = n; i--;)
            p[--c3[tmp[i] >> b * 3 & mask]] = tmp[i];
        if (sizeof(T) == 8) {
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[p[i] >> b * 4 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                tmp[--c0[p[i] >> b * 4 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[tmp[i] >> b * 5 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                p[--c0[tmp[i] >> b * 5 & mask]] = tmp[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[p[i] >> b * 6 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                tmp[--c0[p[i] >> b * 6 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[tmp[i] >> b * 7 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                p[--c0[tmp[i] >> b * 7 & mask]] = tmp[i];
        }
        if (std::is_signed<T>::value) {
            int i = n;
            while (i && p[i - 1] < 0)
                i--;
            std::rotate(p, p + i, p + n);
        }
    }
    template <typename T>
    void radix_sort(T& v) { radix_sort(v.size(), v.data()); }
    struct less_by_first {
        template <typename T, typename U>
        bool operator()(T&& s, U&& t) const {
            return s.first < t.first;
        }
    };
    template <typename T, typename U>
    void radix_sort_compare_first(int n, std::pair<T, U>* p) {
        static_assert(sizeof(T) == 4 || sizeof(T) == 8, "");
        if (!n)
            return;
        if (n <= 64) {
            std::stable_sort(p, p + n, less_by_first{});
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
        for (int i = 0; i < n; i++)
            c0[p[i].first & mask]++;
        for (int i = 0; i < powb - 1; i++)
            c0[i + 1] += c0[i];
        for (int i = n; i--;)
            tmp[--c0[p[i].first & mask]] = p[i];
        memset(c0, 0, sizeof(c0));
        for (int i = 0; i < n; i++)
            c0[tmp[i].first >> b & mask]++;
        for (int i = 0; i < powb - 1; i++)
            c0[i + 1] += c0[i];
        for (int i = n; i--;)
            p[--c0[tmp[i].first >> b & mask]] = tmp[i];
        memset(c0, 0, sizeof(c0));
        for (int i = 0; i < n; i++)
            c0[p[i].first >> b * 2 & mask]++;
        for (int i = 0; i < powb - 1; i++)
            c0[i + 1] += c0[i];
        for (int i = n; i--;)
            tmp[--c0[p[i].first >> b * 2 & mask]] = p[i];
        memset(c0, 0, sizeof(c0));
        for (int i = 0; i < n; i++)
            c0[tmp[i].first >> b * 3 & mask]++;
        for (int i = 0; i < powb - 1; i++)
            c0[i + 1] += c0[i];
        for (int i = n; i--;)
            p[--c0[tmp[i].first >> b * 3 & mask]] = tmp[i];

        if (sizeof(T) == 8) {
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[p[i].first >> b * 4 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                tmp[--c0[p[i].first >> b * 4 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[tmp[i].first >> b * 5 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                p[--c0[tmp[i].first >> b * 5 & mask]] = tmp[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[p[i].first >> b * 6 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                tmp[--c0[p[i].first >> b * 6 & mask]] = p[i];
            memset(c0, 0, sizeof(c0));
            for (int i = 0; i < n; i++)
                c0[tmp[i].first >> b * 7 & mask]++;
            for (int i = 0; i < powb - 1; i++)
                c0[i + 1] += c0[i];
            for (int i = n; i--;)
                p[--c0[tmp[i].first >> b * 7 & mask]] = tmp[i];
        }

        if (std::is_signed<T>::value) {
            int i = n;
            while (i && p[i - 1].first < 0)
                i--;
            std::rotate(p, p + i, p + n);
        }
    }
    template <typename T>
    void radix_sort_compare_first(T& v) { radix_sort_compare_first(v.size(), v.data()); }
} // namespace RadixSortImpl

using RadixSortImpl::radix_sort;
using RadixSortImpl::radix_sort_compare_first;
