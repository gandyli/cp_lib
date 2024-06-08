#pragma once
#include "template.hpp"

constexpr auto itos_table = [] {
    std::array<char, 40000> O;
    char* p = O.data();
    _for (i, 10)
        _for (j, 10)
            _for (k, 10)
                _for (l, 10)
                    *p++ = 48 + i, *p++ = 48 + j, *p++ = 48 + k, *p++ = 48 + l;
    return O;
}();