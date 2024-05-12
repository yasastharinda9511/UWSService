//
// Created by yasas on 5/11/24.
//

#include <future>

namespace Basics::ExtendedPromise{

    template<typename T>
    class ExtendedPromise {

    public:
        ExtendedPromise(): _future(_promise.get_future()){}
        ExtendedPromise<T> on_success(std::function<void(const T&)> on_success ){
            _on_success = std::move(on_success);
            return this;
        }

        ExtendedPromise<T>  set_on_fail(std::function<void(const std::exception&)> callback) {
            _on_fail= std::move(callback);
            return this;
        }

        void complete(const T& value){
            _promise.set_value(value);
            if (_on_success) {
                std::thread([this, value]() {
                    _on_success(value);
                }).detach();
            }
        }
        void fail(const std::exception& ex){
            _promise.set_exception(ex);
            std::thread([this, &ex]() {
                _on_fail(ex);
            }).detach();

        }

        std::promise<T> _promise;
        std::future<T> _future;
        std::function<void(const T&)> _on_success;
        std::function<void(const std::exception&)> _on_fail;
    };
}