#pragma once

#include "template.hpp"

#ifdef CHECK_EOF
#define ECHK6 c <= ' ' && c > 0
#define ECHK5 *ptr ? *ptr : -1
#define ECHK0 *ptr ? *ptr++ : -1
#define ECHK1     \
    if (ch == -1) \
        return set(false), *this;
#define ECHK2  \
    if (!*ptr) \
        return set(false), *this;
#define ECHK3 &&ch != -1
#define ECHK4 &&*ptr
#else
#define ECHK6 c <= ' '
#define ECHK5 *ptr
#define ECHK0 *ptr++
#define ECHK1
#define ECHK2
#define ECHK3
#define ECHK4
#endif

#if defined(__unix__) && !defined(LX_DEBUG) && !defined(LX_LOCAL)
#define USE_MMAP
#include <sys/mman.h>
#include <sys/stat.h>
#endif

#if defined(CHECK_EOF) || !defined(USE_MMAP)
#define EV (-1)
#else
#define EV 0
#endif

constexpr usize bufSize = 1 << 20;

class IO;
class In {
    friend class IO;

private:
    FILE* inFile;
    bool status = true;
#ifdef USE_MMAP
    struct stat st;
    char* ptr;
    int fd;
#elif !defined(LX_DEBUG)
    char buf[bufSize], *p1, *p2;
#endif
    static constexpr bool isdigit(int c) { return '0' <= c && c <= '9'; }

public:
#ifdef LX_DEBUG
    int getch() { return fgetc(inFile); }
    int peek() {
        int c = fgetc(inFile);
        ungetc(c, inFile);
        return c;
    }
    void input(FILE* f) { inFile = f, set(); }
#elif defined(USE_MMAP)
    int getch() { return ECHK0; }
    int peek() { return ECHK5; }
    void input(FILE* f) {
        inFile = f;
        if (inFile)
            fd = fileno(inFile), fstat(fd, &st), ptr = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0), set();
    }
    static const std::array<u32, 0x10000> n;
#else
    int getch() { return (p1 == p2 ? (p2 = (p1 = buf) + fread(buf, 1, bufSize, inFile)) : 0), p1 == p2 ? -1 : *p1++; }
    int peek() { return (p1 == p2 ? (p2 = (p1 = buf) + fread(buf, 1, bufSize, inFile)) : 0), p1 == p2 ? -1 : *p1; }
    void input(FILE* f) { inFile = f, p1 = p2 = buf, set(); }
#endif
    void input(const char* s) { input(fopen(s, "rb")); }
    void set(bool s = true) { status = s; }
    bool get() const { return status; }
};
#if !defined(LX_DEBUG) && defined(USE_MMAP)
const std::array<u32, 0x10000> In::n = []() {
    std::array<u32, 0x10000> n{};
    std::fill(all(n), -1);
    constexpr u32 e0 = 0x01, e1 = 0x100;
    int x = 0;
    for (u32 i = 0, c0 = 0x3030; i != 10; i++, c0 += e0)
        for (u32 j = 0, c1 = c0; j != 10; j++, c1 += e1)
            n[c1] = x++;
    return n;
}();
#endif
class Out {
    friend class IO;

private:
    FILE* outFile;
#ifndef LX_DEBUG
    char pbuf[bufSize], *pp = pbuf;
#endif

public:
#ifdef LX_DEBUG
    void flush() { fflush(outFile); }
    void putch(char c) { fputc(c, outFile); }
    void writestr(const char* s, usize n) { fwrite(s, 1, n, outFile); }
#else
    void flush() { fwrite(pbuf, 1, pp - pbuf, outFile), pp = pbuf; }
    void putch(char c) { ((pp - pbuf == bufSize) ? flush() : void(0)), *pp++ = c; }
    void writestr(const char* s, usize n) {
        if (n >= bufSize - (pp - pbuf))
            flush(), fwrite(s, 1, n, outFile);
        else
            memcpy(pp, s, n), pp += n;
    }
    static const std::array<u32, 10000> D;
#endif
    void output(const char* s) { output(fopen(s, "wb")); }
    void output(FILE* f) { outFile = f; }
};
#ifndef LX_DEBUG
const std::array<u32, 10000> Out::D = []() {
    constexpr u32 e0 = 0x1, e1 = 0x100, e2 = 0x10000, e3 = 0x1000000;
    std::array<u32, 10000> m{};
    int x = 0;
    for (u32 i = 0, c0 = 0x30303030; i != 10; i++, c0 += e0)
        for (u32 j = 0, c1 = c0; j != 10; j++, c1 += e1)
            for (u32 k = 0, c2 = c1; k != 10; k++, c2 += e2)
                for (u32 l = 0, c3 = c2; l != 10; l++, c3 += e3)
                    m[x++] = c3;
    return m;
}();
#endif
template <typename... Args>
struct has_multiple_args {
    static constexpr bool value = sizeof...(Args) > 0;
};
template <typename T, typename IO>
struct has_read_method {
    template <typename U>
    static auto test(U* p) -> decltype(p->read(std::declval<IO>()), std::true_type());
    template <typename U>
    static auto test(...) -> std::false_type;
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};
template <typename T, typename IO>
struct has_write_method {
    template <typename U>
    static auto test(U* p) -> decltype(p->write(std::declval<IO>()), std::true_type());
    template <typename U>
    static auto test(...) -> std::false_type;
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};
class IO: public In, public Out {
private:
    u32 precision = 12;

public:
    IO(FILE* i = stdin, FILE* o = stdout) { input(i), output(o); }
    ~IO() { flush(); }
    static constexpr bool blank(char c) { return ECHK6; }
    template <typename T, typename... Args>
    typename std::enable_if<has_multiple_args<Args...>::value, IO&>::type read(T& x, Args&... args) {
        read(x), read(args...);
        return *this;
    }
    template <typename T>
    typename std::enable_if<Signed<T>::value, IO&>::type read(T& x) {
        x = 0;
        static typename make_unsigned<T>::type t;
        bool sign = false;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            sign = (ch == '-'), ch = getch();
        ECHK1
        t = 0;
        while (isdigit(ch))
            t = t * 10 + (ch ^ 48), ch = getch();
#else
        while (!isdigit(*ptr) ECHK4)
            sign = *ptr++ == '-';
        ECHK2
        t = *ptr++ ^ 48;
        while (~n[*reinterpret_cast<u16*&>(ptr)])
            t = t * 100 + n[*reinterpret_cast<u16*&>(ptr)++];
        if (isdigit(*ptr))
            t = t * 10 + (*ptr++ ^ 48);
#endif
        x = sign ? (~t + 1) : t;
        return *this;
    }
    template <typename T>
    typename std::enable_if<Unsigned<T>::value, IO&>::type read(T& x) {
        x = 0;
#ifndef USE_MMAP
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            ch = getch();
        ECHK1
        while (isdigit(ch))
            x = (x << 1) + (x << 3) + (ch ^ 48), ch = getch();
#else
        while (!isdigit(*ptr) ECHK4)
            ptr++;
        ECHK2
        x = *ptr++ ^ 48;
        while (~n[*reinterpret_cast<u16*&>(ptr)])
            x = x * 100 + n[*reinterpret_cast<u16*&>(ptr)++];
        if (isdigit(*ptr))
            x = x * 10 + (*ptr++ ^ 48);
#endif
        return *this;
    }
    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, IO&>::type read(T& x) {
        x = 0;
        T t = 1;
        bool sign = false;
        int ch = getch();
        while (!isdigit(ch) ECHK3)
            sign = (ch == '-'), ch = getch();
        ECHK1
        while (isdigit(ch))
            x = x * 10 + (ch ^ 48), ch = getch();
        if (ch == '.')
            for (ch = getch(); isdigit(ch); ch = getch())
                x += (t /= 10) * (ch ^ 48);
        x = sign ? -x : x;
        return *this;
    }
    template <typename T>
    T read() {
        static typename std::decay<T>::type x;
        return read(x), x;
    }
    IO& read(char& ch) {
        do
            ch = getch();
        while (blank(ch));
        ECHK1
        return *this;
    }
    IO& read(char* s) {
        int ch = getch();
        while (blank(ch))
            ch = getch();
        ECHK1
        while (!blank(ch))
            *s++ = ch, ch = getch();
        *s = 0;
        return *this;
    }
    IO& read(std::string& s) {
        int ch = getch();
        while (blank(ch))
            ch = getch();
        ECHK1
        s.erase();
        while (!blank(ch))
            s.append(1, ch), ch = getch();
        return *this;
    }
    IO& readline(char* s) {
        char* t = s;
        int ch = getch();
        while (ch != '\n' && ch != EV)
            *s++ = ch, ch = getch();
        *s = 0;
        if (s == t && ch != '\n')
            set(false);
        return *this;
    }
    IO& readline(std::string& s) {
        s.erase();
        int ch = getch();
        while (ch != '\n' && ch != EV)
            s.append(1, ch), ch = getch();
        if (s.empty() && ch != '\n')
            set(false);
        return *this;
    }
    template <size_t N = 0, typename T>
    typename std::enable_if<N == std::tuple_size<T>::value>::type read_tuple(T&) {}
    template <size_t N = 0, typename T>
      typename std::enable_if < N<std::tuple_size<T>::value>::type read_tuple(T& t) {
        if (N < std::tuple_size<T>::value) {
            read(std::get<N>(t));
            read_tuple<N + 1>(t);
        }
    }
    template <typename T>
    typename std::enable_if<tupleLike<T>::value, IO&>::type read(T& t) {
        return read_tuple(t), *this;
    }
    template <typename R>
    typename std::enable_if<forward_range<R>::value, IO&>::type read(R&& r) { return readArray(std::forward<R>(r)); }
    template <typename T>
    typename std::enable_if<has_read_method<T, IO&>::value, IO&>::type read(T& t) { return t.read(*this), *this; }
    void setprecision(u32 n = 6) { precision = n; }
    template <typename T, typename... Args>
    typename std::enable_if<has_multiple_args<Args...>::value>::type write(T&& x, Args&&... args) {
        write(std::forward<T>(x));
        write(std::forward<Args>(args)...);
    }
    void write() const {}
    template <typename T>
    typename std::enable_if<Signed<T>::value>::type write(T x) {
        static typename make_unsigned<T>::type y;
        y = x;
        if (x < 0)
            putch('-'), write(~y + 1);
        else
            write(y);
    }
    static constexpr u64 L(int x) { return x == 1 ? 0 : ten(x - 1); }
    static constexpr u64 R(int x) { return ten(x) - 1; }
    template <typename T>
    typename std::enable_if<std::is_unsigned<T>::value>::type write(T x) {
#ifndef LX_DEBUG
        if (std::end(pbuf) - pp < 64)
            flush();

#define de(t)                            \
    case L(t)... R(t):                   \
        *(u32*)(pp) = D[x / ten((t)-4)]; \
        pp += 4;                         \
        x %= ten((t)-4);

        u64 y = x;
        switch (y) {
            de(18);
            de(14);
            de(10);
            de(6);
        case L(2)... R(2):
            *(u32*)(pp) = D[x * 100];
            pp += 2;
            break;

            de(17);
            de(13);
            de(9);
            de(5);
        case L(1)... R(1):
            *pp = x ^ 48;
            pp += 1;
            break;

        default:
            *(u32*)(pp) = D[x / ten(16)];
            pp += 4;
            x %= ten(16);
            de(16);
            de(12);
            de(8);
        case L(4)... R(4):
            *(u32*)pp = D[x];
            pp += 4;
            break;

            de(19);
            de(15);
            de(11);
            de(7);
        case L(3)... R(3):
            *(u32*)(pp) = D[x * 10];
            pp += 3;
            break;
        }
#undef de
#else
        write(u128(x));
#endif
    }
    void write(u128 x) {
        static int s[40], t = 0;
        do
            s[t++] = x % 10, x /= 10;
        while (x);
        while (t)
            putch(s[--t] ^ 48);
    }
    void write(char c) { putch(c); }
    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value>::type write(T x) {
        static char buf[512];
        std::sprintf(buf, "%.*Lf", precision, ld(x));
        write(buf);
    }
    void write(bool x) { putch(x ^ 48); }
    void write(char* s) { writestr(s, strlen(s)); }
    void write(const char* s) { writestr(s, strlen(s)); }
    void write(const std::string& s) { writestr(s.data(), len(s)); }
    template <typename T>
    void write(std::initializer_list<T> t) {
        auto f = std::begin(t), l = std::end(t);
        if (f != l)
            for (write(*f++); f != l; ++f) {
                putch(' ');
                write(*f);
            }
    }
    template <size_t N = 0, typename T>
    typename std::enable_if<N == std::tuple_size<typename std::decay<T>::type>::value>::type write_tuple(T&&) {}
    template <size_t N = 0, typename T>
      typename std::enable_if < N<std::tuple_size<typename std::decay<T>::type>::value>::type write_tuple(T&& t) {
        if (N < std::tuple_size<typename std::decay<T>::type>::value) {
            if (N)
                putch(' ');
            write(std::get<N>(std::forward<T>(t)));
            write_tuple<N + 1>(std::forward<T>(t));
        }
    }
    template <typename T>
    typename std::enable_if<tupleLike<T>::value>::type write(T&& t) {
        write_tuple(std::forward<T>(t));
    }
    template <typename R>
    typename std::enable_if<forward_range<R>::value>::type write(R&& r) {
        if (std::is_same<typename std::decay<R>::type, std::string>::value)
            return write(r.data());
        auto f = begin(r), l = end(r);
        if (f != l)
            for (write(*f++); f != l; ++f) {
                putch(' ');
                write(*f);
            }
    }
    template <typename T>
    typename std::enable_if<has_write_method<T, IO&>::value, IO&>::type write(T&& t) { t.write(*this); }
    template <typename... Args>
    void writeln(Args&&... x) { write(std::forward<Args>(x)...), putch('\n'); }
    template <typename I>
    typename std::enable_if<is_forward_iterator<I>::value, IO&>::type readArray(I f, I l) {
        for (; f != l; ++f)
            read(*f);
        return *this;
    }
    template <typename R>
    typename std::enable_if<forward_range<R>::value, IO&>::type readArray(R&& r) { return readArray(std::begin(r), std::end(r)); }
    template <typename I>
    typename std::enable_if<is_forward_iterator<I>::value>::type displayArray(I f, I l, char d = ' ') {
        if (f != l)
            for (write(*f++); f != l; ++f) {
                putch(d);
                write(*f);
            }
        putch('\n');
    }
    template <typename R>
    typename std::enable_if<forward_range<R>::value>::type displayArray(R&& r, char d = ' ') { return displayArray(std::begin(r), std::end(r), d); }
    operator bool() const { return get(); }
} io;
#ifdef LX_LOCAL
IO err(nullptr, stderr);
#define dbg(x) err.write(#x " = "), err.writeln(x)
#else
#define dbg(x) \
    do {       \
    } while (false)
#endif
template <typename F>
void multipleTests(F&& f, IO& io = ::io) {
    dR(u32, q);
    _for (q)
        f();
}
template <typename... Args>
void writeln(Args&&... x) { io.writeln(std::forward<Args>(x)...); }
template <typename T>
void writeln(std::initializer_list<T> x) { io.writeln(x); }
void YN(bool v, bool c = true) {
    if (c)
        return writeln(v ? "YES" : "NO");
    return writeln(v ? "Yes" : "No");
}
