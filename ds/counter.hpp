#pragma once
#include "hashmap.hpp"

template <typename T>
struct Counter: HashMap<T, int> {
    Counter() = default;
    template <input_range R>
    Counter(R&& r) {
        foreach (x, r)
            (*this)[x]++;
    }
    Counter(std::initializer_list<T> il) {
        foreach (x, il)
            (*this)[x]++;
    }
    i64 total() const { return sum(this->values(), 0LL); }
    auto most_common(int n) const {
        if (n == 1)
            return vc{max(*this, [&](auto&& a, auto&& b) { return a.second < b.second; })};
        if (n == this->size()) {
            auto res = this->items();
            sort(res, [&](auto&& a, auto&& b) { return a.second > b.second; });
            res.resize(n);
            return res;
        }
        int N = this->size();
        auto it = this->begin();
        vc<std::pair<std::pair<T, int>, int>> heap(n);
        _for (i, n)
            heap[i] = {*it++, -i};
        make_heap(heap);
        int top = heap[0].first.second;
        int order = -n;
        _for (i, n, N) {
            auto e = *it++;
            if (top < e.second) {
                pop_heap(heap);
                heap[n - 1] = {e, order};
                push_heap(heap);
                top = heap[0].first.second;
                order--;
            }
        }
        sort_heap(heap);
        vc<std::pair<T, int>> res(n);
        _for (i, n)
            res[i] = heap[i].first;
        return res;
    }
    auto most_common() const { return most_common(this->size()); }
    vc<T> elements() const {
        vc<T> res;
        foreach (k, v, *this)
            res.insert(res.end(), v, k);
        return res;
    }
    Counter& operator+=(const Counter& rhs) {
        foreach (k, v, rhs)
            (*this)[k] += v;
        return *this;
    }
    Counter& operator-=(const Counter& rhs) {
        foreach (k, v, rhs)
            (*this)[k] -= v;
        return *this;
    }
    Counter operator+(const Counter& rhs) const { return Counter(*this) += rhs; }
    Counter operator-(const Counter& rhs) const { return Counter(*this) -= rhs; }
};
template <input_range R>
Counter(R&& r) -> Counter<range_value_t<R>>;