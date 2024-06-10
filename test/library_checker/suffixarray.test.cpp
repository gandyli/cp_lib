#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include "all.hpp"
#include "string/suffix_array.hpp"

int main() {
    dR(str, s);
    Suffix_Array SA(s);
    print(SA.sa);
    return 0;
}