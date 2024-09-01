//
// Created by yasas on 5/11/24.
//
#pragma  once

#include <future>
#include "LockFreeQueue/LockFreeQueue.h"

namespace async {

    template<typename T>
    class ExtendedPromise {

    public:
        ExtendedPromise() : _future(_promise.get_future()) {}

        ExtendedPromise<T> *on_success(std::function<void(const T &)> on_success) {
            _on_success = std::move(on_success);
            return this;
        }

        ExtendedPromise<T> *on_fail(std::function<void(const std::exception_ptr)> callback) {
            _on_fail = std::move(callback);
            return this;
        }

        void complete(const T &value) {
            _promise.set_value(value);
            if (_on_success) {
                _on_success(value);
            }
        }

        void fail(std::exception_ptr ex) {
            _promise.set_exception(ex);
            if (_on_fail) {
                _on_fail(ex);
            }
        }

        template<typename Func>
        auto compose(Func func) -> std::shared_ptr<ExtendedPromise<decltype(func(std::declval<T>()))>>{

            using NextReturnType = decltype(func(std::declval<T>()));
            auto next_promise = std::make_shared<ExtendedPromise<NextReturnType>>();

            this->on_success([func, next_promise](const T& result) mutable {
                try {
                    NextReturnType next_result = func(result);
                    next_promise->complete(next_result);
                } catch (...) {
                    next_promise->fail(std::current_exception());
                }
            });

            this->on_fail([next_promise](std::exception_ptr ex) mutable {
                next_promise->fail(ex);
            });

            return next_promise;

        }

    private:
        std::promise<T> _promise;
        std::future<T> _future;
        std::function<void(const T &)> _on_success;
        std::function<void(const T &)> _compose;
        std::function<void(const std::exception_ptr)> _on_fail;
    };
}