#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"

#include "all.hpp"
#include "ds/removable_queue.hpp"

int main() {
    dR(int, n, q);
    dRV(int, a, n);
    RemovableQueue<PQG<int>> q1(a);
    RemovableQueue<PQ<int>> q2(a);
    _for (q) {
        dR(char, t);
        if (t == '0') {
            dR(int, x);
            q1.push(x), q2.push(x);
        }
        else if (t == '1') {
            int x = q1.pop();
            q2.remove(x);
            print(x);
        }
        else {
            int x = q2.pop();
            q1.remove(x);
            print(x);
        }
    }
    return 0;
}