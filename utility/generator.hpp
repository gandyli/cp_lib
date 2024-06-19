#pragma once
#include "template.hpp"
#include <coroutine>

template <typename T>
class generator;

namespace detail {
    template <typename T>
    class generator_promise {
    public:
        using value_type = std::remove_reference_t<T>;
        using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
        using pointer_type = value_type*;

        generator_promise() = default;
        generator<T> get_return_object() noexcept;
        auto initial_suspend() const { return std::suspend_always{}; }
        auto final_suspend() const noexcept { return std::suspend_always{}; }

        template <typename U = T, std::enable_if_t<!std::is_rvalue_reference<U>::value, int> = 0>
        auto yield_value(std::remove_reference_t<T>& value) noexcept {
            m_value = std::addressof(value);
            return std::suspend_always{};
        }
        auto yield_value(std::remove_reference_t<T>&& value) noexcept {
            m_value = std::addressof(value);
            return std::suspend_always{};
        }
        void unhandled_exception() { m_exception = std::current_exception(); }
        void return_void() noexcept {}
        reference_type value() const noexcept { return static_cast<reference_type>(*m_value); }

        template <typename U>
        std::suspend_never await_transform(U&& value) = delete;

        void rethrow_if_exception() {
            if (m_exception) {
                std::rethrow_exception(m_exception);
            }
        }

    private:
        pointer_type m_value{nullptr};
        std::exception_ptr m_exception;
    };

    struct generator_sentinel {
    };

    template <typename T>
    class generator_iterator {
        using coroutine_handle = std::coroutine_handle<generator_promise<T>>;

    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = generator_promise<T>::value_type;
        using reference = generator_promise<T>::reference_type;
        using pointer = generator_promise<T>::pointer_type;

        generator_iterator() noexcept = default;

        explicit generator_iterator(coroutine_handle coroutine) noexcept: m_coroutine(coroutine) {}

        friend bool operator==(const generator_iterator& it, generator_sentinel) noexcept {
            return it.m_coroutine == nullptr || it.m_coroutine.done();
        }
        friend bool operator!=(const generator_iterator& it, generator_sentinel s) noexcept { return !(it == s); }
        friend bool operator==(generator_sentinel s, const generator_iterator& it) noexcept { return (it == s); }
        friend bool operator!=(generator_sentinel s, const generator_iterator& it) noexcept { return it != s; }

        generator_iterator& operator++() {
            m_coroutine.resume();
            if (m_coroutine.done()) {
                m_coroutine.promise().rethrow_if_exception();
            }
            return *this;
        }

        void operator++(int) { (void)operator++(); }
        reference operator*() const noexcept { return m_coroutine.promise().value(); }
        pointer operator->() const noexcept { return std::addressof(operator*()); }

    private:
        coroutine_handle m_coroutine{nullptr};
    };

} // namespace detail

template <typename T>
class generator {
public:
    using promise_type = detail::generator_promise<T>;
    using iterator = detail::generator_iterator<T>;
    using sentinel = detail::generator_sentinel;

    generator() noexcept: m_coroutine(nullptr) {}

    generator(const generator&) = delete;
    generator(generator&& other) noexcept: m_coroutine(other.m_coroutine) { other.m_coroutine = nullptr; }

    auto operator=(const generator&) = delete;
    generator& operator=(generator&& other) noexcept {
        m_coroutine = other.m_coroutine;
        other.m_coroutine = nullptr;

        return *this;
    }
    ~generator() {
        if (m_coroutine) {
            m_coroutine.destroy();
        }
    }
    iterator begin() {
        if (m_coroutine != nullptr) {
            m_coroutine.resume();
            if (m_coroutine.done()) {
                m_coroutine.promise().rethrow_if_exception();
            }
        }
        return iterator{m_coroutine};
    }
    sentinel end() noexcept { return {}; }

private:
    friend class detail::generator_promise<T>;

    explicit generator(std::coroutine_handle<promise_type> coroutine) noexcept: m_coroutine(coroutine) {}

    std::coroutine_handle<promise_type> m_coroutine;
};

namespace detail {
    template <typename T>
    generator<T> generator_promise<T>::get_return_object() noexcept {
        return generator<T>{std::coroutine_handle<generator_promise<T>>::from_promise(*this)};
    }
} // namespace detail
