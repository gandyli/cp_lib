#pragma once
#include "template.hpp"

struct FwriteWriter {
    FILE* out;
    char obuf[1 << 20], *op = obuf;
    FwriteWriter(FILE* f): out(f) {}
    FwriteWriter(std::string_view s): FwriteWriter(fopen(s.data(), "w")) {}
    void putch(char c) {
        if (op == end(obuf))
            flush();
        putch_unchecked(c);
    }
    void putch_unchecked(char c) { *op++ = c; }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    template <int N>
    void ensure() {
        if (end(obuf) - op < N) [[unlikely]]
            flush();
    }
};