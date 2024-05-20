#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "all.hpp"
#include "date.hpp"

void test() {
    using DT = Date<int>;
    const Date d(2014, 5, 17);
    [[maybe_unused]] Date<int> tmp;
    Date d1(d);
    Date d2(d1.to_int());

    assert(d1 == d2);

    d1++;
    assert(d1 > d2);

    assert(DT::is_leap_year(2000));
    assert(!DT::is_leap_year(1900));
    assert(DT::is_leap_year(2004));

    // test Date::to_string
    assert(d1.to_string() == "2014-05-18");
    assert(d1.to_string("/") == "2014/05/18");

    d1 += 1000;
    assert(d1 - d2 == 1001);
    assert(d2 - d1 == -1001);

    d1 -= 1001;

    assert(d1.weekday() == 6);
    assert(DT(2012, 2, 29).weekday() == 3);

    assert(d - DT(1988, 7, 3) == 9449);
    assert(d - DT(1, 1, 1) == 735369);
    assert(d - DT(2014, 5, 16) == 1);
    assert(d - DT(2012, 2, 29) == 808);

    _for (i, ten(6)) {
        assert(DT(d + i).to_int() == i + 735369);
    }
    d1--;
    assert(d - 1 == d1);
}
int main() {
    test();

    dR(int, a, b);
    print(a + b);
    return 0;
}