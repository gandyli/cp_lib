#pragma once
#include "template.hpp"

struct DummyReader {
    static constexpr int ev{};
    DummyReader(auto) {}
    int getch();
    int getch_unchecked();
    int peek() const;
    void ireadstr(char*, usize);
    void skipws();
    void read(char*);
    void read(str&);
    void readstr(char*, usize);
    void readstr(str&, usize);
    char*& raw_ip();
    template <usize>
    void ensure();
};
struct DummyWriter {
    DummyWriter(auto) {}
    void putch(char);
    void putch_unchecked(char);
    void writestr(const char*, usize);
    void flush() {}
    template <usize>
    void ensure();
};