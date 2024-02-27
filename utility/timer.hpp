#pragma once
#include "template.hpp"

struct Timer {
    std::chrono::high_resolution_clock::time_point st;

    Timer() { reset(); }
    void reset() { st = std::chrono::high_resolution_clock::now(); }

    auto operator()() const {
        auto ed = std::chrono::high_resolution_clock::now();
        return ed - st;
    }
};