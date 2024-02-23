#pragma once
#include "template.hpp"

constexpr auto itos_table = [] {
    std::array<u32, 10000> O{};
    int x = 0;
    _for (i, 10)
        _for (j, 10)
            _for (k, 10)
                _for (l, 10)
                    O[x++] = i + j * 0x100 + k * 0x10000 + l * 0x1000000 + 0x30303030;
    return O;
}();