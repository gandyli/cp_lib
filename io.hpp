#pragma once
#include "utility/itos_table.hpp"

#ifndef FASTIO
#define FASTIO 1

#if defined(__unix__) && !defined(LX_LOCAL)
#define USE_MMAP
#define EV 0
#include <sys/mman.h>
#include <sys/stat.h>
#else
#define EV (-1)
#endif

struct IO {
    static constexpr usize bufSize = 1 << 20;
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }
    static constexpr bool blank(int c) { return c <= ' '; }

    u32 prec = 12;
    FILE *in, *out;
    char obuf[bufSize], *ip, *op = obuf;

#ifndef USE_MMAP
    char ibuf[bufSize + 8], *eip;
    bool eoi = false;
    void load() {
        if (eoi) [[unlikely]]
            return;
        usize sz = eip - ip;
        memcpy(ibuf, ip, sz);
        eip = ibuf + sz + fread(ibuf + sz, 1, bufSize - sz, in);
        if (eip != ibuf + bufSize) [[unlikely]]
            eoi = true;
        ip = ibuf;
    }
    void skipws() {
        int ch = getch();
        while (blank(ch))
            ch = getch();
        ip--;
    }
    int getch() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return (ip == eip ? load() : void()), ip == eip ? -1 : *ip; }
    void input(FILE* f) { in = f, ip = eip = ibuf; }
    void ireadstr(char* s, usize n) {
        if (usize len = eip - ip; n > len) [[unlikely]] {
            memcpy(s, ip, len);
            n -= len;
            s += len;
            ip = eip;
            fread(s, 1, n, in);
        }
        else
            memcpy(s, ip, n), ip += n;
    }
#else
    void skipws() {
        while (blank(*ip))
            ip++;
    }
    int getch() { return *ip++; }
    int getch_unchecked() { return *ip++; }
    int peek() { return *ip; }
    void input(FILE* f) {
        struct stat st;
        int fd;
        in = f;
        if (in)
            fd = fileno(in), fstat(fd, &st), ip = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    }
    void ireadstr(char* s, usize n) { memcpy(s, ip, n), ip += n; }
#endif
    void input(std::string_view s) { input(fopen(s.data(), "rb")); }
    void set(bool = true) {}
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    IO& read(Args&... x) {
        (read(x), ...);
        return *this;
    }
    template <Unsigned T>
    void parse_int(T& x) {
        loop {
            u64 v;
            memcpy(&v, ip, 8);
            if ((v -= 0x3030303030303030) & 0x8080808080808080)
                break;
            v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0xffffffff;
            x = 100000000 * x + v;
            ip += 8;
        }
        {
            u32 v;
            memcpy(&v, ip, 4);
            if (!((v -= 0x30303030) & 0x80808080)) {
                v = (v * 10 + (v >> 8)) & 0xff00ff;
                v = (v * 100 + (v >> 16)) & 0xffff;
                x = 10000 * x + v;
                ip += 4;
            }
        }
        {
            u16 v;
            memcpy(&v, ip, 2);
            if (!((v -= 0x3030) & 0x8080)) {
                v = (v * 10 + (v >> 8)) & 0xff;
                x = 100 * x + v;
                ip += 2;
            }
        }
        if (isdigit(*ip))
            x = 10 * x + (*ip++ ^ 48);
    }
    template <Signed T>
    IO& read(T& x) {
        skipws();
#ifndef USE_MMAP
        if (eip - ip < 64) [[unlikely]]
            load();
#endif
        make_unsigned_t<T> t{};
        if (*ip == '-') {
            ip++;
            parse_int(t);
            t = -t;
        }
        else
            parse_int(t);
        x = t;
        return *this;
    }
    IO& read(Unsigned auto& x) {
        x = 0;
        skipws();
#ifndef USE_MMAP
        if (eip - ip < 64) [[unlikely]]
            load();
#endif
        parse_int(x);
        return *this;
    }
    IO& read(std::floating_point auto& x) {
        static str s;
        if (read(s))
            std::from_chars(s.begin().base(), s.end().base(), x);
        return *this;
    }
    template <typename T = int>
    T read() {
        std::decay_t<T> x;
        return read(x), x;
    }
    IO& read(char& ch) {
        skipws();
        ch = getch();
        return *this;
    }
#ifdef USE_MMAP
    usize next_size() const {
        char* ip = this->ip;
        while (!blank(*ip))
            ip++;
        return ip - this->ip;
    }
    IO& read(char* s) {
        skipws();
        auto n = next_size();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        auto n = next_size();
        s.assign(ip, n);
        ip += n;
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.assign(ip, n);
        ip += n;
        return *this;
    }
#else
    IO& read(char* s) {
        skipws();
        int ch = peek();
        while (!blank(ch))
            *s++ = ch, getch_unchecked(), ch = peek();
        *s = 0;
        return *this;
    }
    IO& read(str& s) {
        skipws();
        int ch = peek();
        s.clear();
        while (!blank(ch))
            s.push_back(ch), getch_unchecked(), ch = peek();
        return *this;
    }
    IO& readstr(str& s, usize n) {
        skipws();
        s.resize(n);
        ireadstr(s.data(), n);
        return *this;
    }
#endif
    IO& readstr(char* s, usize n) {
        skipws();
        ireadstr(s, n);
        s[n] = 0;
        return *this;
    }
    IO& readline(char* s) {
        int ch = getch();
        while (ch != '\n' && ch != EV)
            *s++ = ch, ch = getch();
        *s = 0;
        return *this;
    }
    IO& readline(str& s) {
        s.clear();
        int ch = getch();
        while (ch != '\n' && ch != EV)
            s.push_back(ch), ch = getch();
        return *this;
    }
    IO& read(tupleLike auto& t) {
        return std::apply([&](auto&... t) { read(t...); }, t), *this;
    }
    IO& read(forward_range auto&& r) { return readArray(FORWARD(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.read(io); }
    IO& read(T& t) { return t.read(*this), *this; }
    template <std::forward_iterator I>
    IO& readArray(I f, I l) {
        while (f != l)
            read(*f++);
        return *this;
    }
    IO& readArray(forward_range auto&& r) { return readArray(all(r)); }
    IO& zipread(auto&&... a) {
        _for (i, (len(a), ...))
            read(a[i]...);
        return *this;
    }

    void flush() { fwrite(obuf, 1, op - obuf, out), op = obuf; }
    void putch_unchecked(char c) { *op++ = c; }
    void putch(char c) { (op == end(obuf) ? flush() : void()), putch_unchecked(c); }
    void writestr(const char* s, usize n) {
        if (n >= usize(end(obuf) - op)) [[unlikely]]
            flush(), fwrite(s, 1, n, out);
        else
            memcpy(op, s, n), op += n;
    }
    void output(std::string_view s) { output(fopen(s.data(), "wb")); }
    void output(FILE* f) { out = f; }
    void setprec(u32 n = 6) { prec = n; }
    template <typename... Args>
    requires (sizeof...(Args) > 1)
    void write(Args&&... x) { (write(FORWARD(x)), ...); }
    void write() {}

    struct WriteInt {
        IO& io;
        template <int N = 4>
        void lead(u64 x) {
            if constexpr (N > 1)
                if (x < ten(N - 1)) {
                    lead<N - 1>(x);
                    return;
                }
            io.op = std::copy_n(&itos_table[x * 4 + (4 - N)], N, io.op);
        }
        template <int N>
        void wt4(u64 x) {
            if constexpr (N > 0) {
                io.op = std::copy_n(&itos_table[x / ten(N - 4) * 4], 4, io.op);
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
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
        make_unsigned_t<T> y = x;
        if (x < 0)
            *op++ = '-', y = -y;
        WriteInt{*this}.write(y);
    }
    void write(Unsigned auto x) {
        if (end(obuf) - op < 64) [[unlikely]]
            flush();
        WriteInt{*this}.write(x);
    }
    void write(char c) { putch(c); }
    void write(std::floating_point auto x) {
        static char buf[512];
        writestr(buf, std::to_chars(buf, buf + 512, x, std::chars_format::fixed, prec).ptr - buf);
    }
    void write(std::string_view s) { writestr(s.data(), s.size()); }
    template <typename I, typename T = std::iter_value_t<I>>
    static constexpr char default_delim = tupleLike<T> || input_range<T> ? '\n' : ' ';
    template <std::input_iterator I, std::sentinel_for<I> S>
    void print_range(I f, S l, char d = default_delim<I>) {
        if (f != l)
            for (write(*f++); f != l; write(d, *f++)) {}
    }
    template <tupleLike T>
    void write(T&& t) {
        std::apply([&](auto&& x, auto&&... y) { write(FORWARD(x)), (write(' ', FORWARD(y)), ...); }, FORWARD(t));
    }
    template <input_range R>
    requires (!std::same_as<range_value_t<R>, char>)
    void write(R&& r) { print_range(all(r)); }
    template <typename T>
    requires requires (T t, IO& io) { t.write(io); }
    void write(T&& t) { t.write(*this); }
    void writeln(auto&&... x) { write(FORWARD(x)...), print(); }
    void print() { putch('\n'); }
    void print(auto&&... x) { write(std::forward_as_tuple(FORWARD(x)...), '\n'); }
    template <std::input_iterator I, std::sentinel_for<I> S>
    void displayArray(I f, S l, char d = default_delim<I>) { print_range(f, l, d), print(); }
    template <input_range R>
    void displayArray(R&& r, char d = default_delim<iterator_t<R>>) { displayArray(all(r), d); }
    operator bool() const { return true; }
} io;
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