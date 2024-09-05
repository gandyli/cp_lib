#pragma once
#include "io/mmapreader.hpp"
#include "io/freadreader.hpp"
#include "io/fwritewriter.hpp"
#include "utility/itos_table.hpp"

#ifdef LX_DEBUG
#include <io/io2.hpp>
#endif

#ifndef FASTIO
#define FASTIO 1

template <typename Reader, typename Writer>
struct IO: Reader, Writer {
    u32 prec = 12;
    IO(FILE* i = stdin, FILE* o = stdout): Reader(i), Writer(o) {}
    ~IO() { this->flush(); }

    using Reader::getch, Reader::read;
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void read(Args&... x) { (read(x), ...); }
    template <Unsigned T>
    void parse_int(T& x) {
        auto& ip = Reader::raw_ip();
        loop {
            u64 v;
            memcpy(&v, ip, 8);
            v -= 0x3030303030303030;
            if (v & 0x8080808080808080)
                break;
            v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0xffffffff;
            ip += 8;
            if constexpr (sizeof(T) < 8) {
                x = v;
                break;
            }
            x = 100000000 * x + v;
        }
        {
            u32 v;
            memcpy(&v, ip, 4);
            v -= 0x30303030;
            if (!(v & 0x80808080)) {
                v = (v * 10 + (v >> 8)) & 0xff00ff;
                v = (v * 100 + (v >> 16)) & 0xffff;
                x = 10000 * x + v;
                ip += 4;
            }
        }
        {
            u16 v;
            memcpy(&v, ip, 2);
            v -= 0x3030;
            if (!(v & 0x8080)) {
                v = (v * 10 + (v >> 8)) & 0xff;
                x = 100 * x + v;
                ip += 2;
            }
        }
        if (*ip >= '0')
            x = 10 * x + (*ip++ ^ 48);
    }
    template <Signed T>
    void read(T& x) {
        this->skipws();
        Reader::template ensure<64>();
        make_unsigned_t<T> t{};
        if (this->peek() == '-') {
            this->getch_unchecked();
            parse_int(t);
            t = -t;
        }
        else
            parse_int(t);
        x = t;
    }
    void read(Unsigned auto& x) {
        this->skipws();
        Reader::template ensure<64>();
        x = 0;
        parse_int(x);
    }
    void read(std::floating_point auto& x) {
        static str s;
        read(s);
        std::from_chars(s.begin().base(), s.end().base(), x);
    }
    template <typename T = int>
    T read() {
        std::decay_t<T> x;
        return read(x), x;
    }
    void read(char& ch) {
        this->skipws();
        ch = getch();
    }
    void read(TupleLike auto& t) {
        std::apply([&](auto&... t) { read(t...); }, t);
    }
    void read(forward_range auto&& r) { return readArray(FORWARD(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.read(io); }
    void read(T& t) { t.read(*this); }
    template <std::forward_iterator I>
    void readArray(I f, I l) {
        while (f != l)
            read(*f++);
    }
    void readArray(forward_range auto&& r) { readArray(all(r)); }
    void zipread(auto&&... a) {
        _for (i, (len(a), ...))
            read(a[i]...);
    }

    void setprec(u32 n) { prec = n; }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() {}
    struct WriteInt {
        Writer& writer;
        template <int N = 4>
        void lead(u64 x) {
            if constexpr (N > 1)
                if (x < ten(N - 1)) {
                    lead<N - 1>(x);
                    return;
                }
            writer.op = std::copy_n(&itos_table[x * 4 + (4 - N)], N, writer.op);
        }
        template <int N>
        void wt4(u64 x) {
            if constexpr (N > 0) {
                writer.op = std::copy_n(&itos_table[x / ten(N - 4) * 4], 4, writer.op);
                wt4<N - 4>(x % ten(N - 4));
            }
        }
        template <int N = 4>
        void wt(u64 x) {
            if constexpr (N < 20)
                if (ten(N) <= x) {
                    wt<N + 4>(x);
                    return;
                }
            lead(x / ten(N - 4));
            wt4<N - 4>(x % ten(N - 4));
        }
        void write(std::unsigned_integral auto x) { wt(x); }
        void write(u128 x) {
            if (x < ten<u128>(16))
                wt(x);
            else if (x < ten<u128>(32)) {
                wt(x / ten<u128>(16));
                wt4<16>(x % ten<u128>(16));
            }
            else {
                wt(x / ten<u128>(32));
                x %= ten<u128>(32);
                wt4<16>(x / ten<u128>(16));
                wt4<16>(x % ten<u128>(16));
            }
        }
    };
    template <Signed T>
    void write(T x) {
        Writer::template ensure<64>();
        make_unsigned_t<T> y = x;
        if (x < 0)
            this->putch_unchecked('-'), y = -y;
        WriteInt{*this}.write(y);
    }
    void write(Unsigned auto x) {
        Writer::template ensure<64>();
        WriteInt{*this}.write(x);
    }
    void write(char c) { this->putch(c); }
    void write(std::floating_point auto x) {
        static char buf[512];
        this->writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
    }
    void write(std::string_view s) { this->writestr(s.data(), s.size()); }
    template <typename I, typename T = std::iter_value_t<I>>
    static constexpr char default_delim = TupleLike<T> || input_range<T> ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
        if (f != l)
            for (write(*f++); f != l; write(d, *f++)) {}
    }
    template <TupleLike T>
    void write(T&& t) {
        std::apply([&](auto&& x, auto&&... y) { write(FORWARD(x)), (write(' ', FORWARD(y)), ...); }, FORWARD(t));
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)..., '\n'); }
    void print() { write('\n'); }
    void print(auto&&... x) { write(std::forward_as_tuple(FORWARD(x)...), '\n'); }
    template <std::input_iterator I, std::sentinel_for<I> S>
    void displayArray(I f, S l, char d = default_delim<I>) { print_range(f, l, d), print(); }
    template <input_range R>
    void displayArray(R&& r, char d = default_delim<iterator_t<R>>) { displayArray(all(r), d); }
};

#if defined(__unix__) && !defined(LX_LOCAL)
using IO_t = IO<MmapReader, FwriteWriter>;
#else
using IO_t = IO<FreadReader, FwriteWriter>;
#endif

inline IO_t io;

#define dR(type, ...) \
    type __VA_ARGS__; \
    io.read(__VA_ARGS__)
#define dRV(type, a, ...)      \
    VEC(type, a, __VA_ARGS__); \
    io.read(a)
#define STR(s, n) \
    str s;        \
    io.readstr(s, n)
void multipleTests(auto&& f) {
    dR(u32, q);
    _for (q)
        f();
}
void writeln(auto&&... x) { io.writeln(FORWARD(x)...); }
void print(auto&&... x) { io.print(FORWARD(x)...); }
void YES(bool v = true) { io.write(v ? "YES\n" : "NO\n"); }
inline void NO(bool v = true) { YES(!v); }
void Yes(bool v = true) { io.write(v ? "Yes\n" : "No\n"); }
inline void No(bool v = true) { Yes(!v); }
#endif