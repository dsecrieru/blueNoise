//
// Created by Daniel Secrieru on 01/04/2023.
// copy&pasted generator from https://en.cppreference.com/w/cpp/language/coroutines
//
#ifndef coro_generator_h
#define coro_generator_h

#include <coroutine>
#include <exception>

namespace coro {

    template<typename T>
    struct generator_t {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type {
            T value_;
            std::exception_ptr exception_;

            generator_t get_return_object() {
                return generator_t(handle_type::from_promise(*this));
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void unhandled_exception() { exception_ = std::current_exception(); }

            template<std::convertible_to<T> from_t>
            std::suspend_always yield_value(from_t&& from) {
                value_ = std::forward<from_t>(from);
                return {};
            }
            void return_void() {}
        };

        handle_type h_;

        explicit generator_t(handle_type h) : h_(h) {}
        ~generator_t() { h_.destroy(); }

        explicit operator bool() {
            fill();
            return !h_.done();
        }
        T operator()() {
            fill();
            full_ = false;
            return std::move(h_.promise().value_);
        }

    private:
        bool full_ = false;

        void fill() {
            if (!full_) {
                h_();
                if (h_.promise().exception_)
                    std::rethrow_exception(h_.promise().exception_);

                full_ = true;
            }
        }
    };

} // namespace coro

#endif // coro_generator_h