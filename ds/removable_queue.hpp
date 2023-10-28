#pragma once
#include "template.hpp"

template <typename Q>
struct RemovableQueue: private Q {
    using T = Q::value_type;
    Q rq;
    int size() const { return Q::size() - len(rq); }
    bool empty() const { return !size(); }

    RemovableQueue() = default;
    RemovableQueue(const Vec<T>& a): Q(all(a)) {}
    T pop() {
        T top = this->top();
        Q::pop();
        return top;
    }
    T top() {
        refresh();
        return Q::top();
    }
    void push(T x) { Q::push(std::move(x)); }
    void remove(T x) { rq.push(std::move(x)); }

private:
    void refresh() {
        while (!rq.empty() && rq.top() == Q::top()) {
            rq.pop();
            Q::pop();
        }
    }
};