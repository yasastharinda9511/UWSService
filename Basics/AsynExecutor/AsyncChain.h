//
// Created by yasas on 5/11/24.
//

#pragma once

#include <future>
#include <thread>
#include "AsyncExceptions/AsyncException.h"

namespace Basics::Async{
    template<typename T>
    class ASyncChain{

    private:
        std::shared_future<T> _future;
        uint32_t _chain_number;

    public:
        ASyncChain():_chain_number(0){}
        explicit ASyncChain(std::shared_future<T> future, uint32_t chain_number): _future(future), _chain_number(chain_number){}

        template<typename Func, typename... Args>
        auto start(Func&& func, Args&&... args){

            auto captured_func = std::forward<Func>(func);
            auto captured_args = std::make_tuple(std::forward<Args>(args)...);

            _future = std::async(std::launch::async, [captured_func, captured_args , this](){
                try{
                    return std::apply(captured_func, std::move(captured_args)); // Apply the function to the result
                }catch (std::exception& exception){
                    throw AsyncChainException (exception.what(), _chain_number);
                }
            });
            return *this;
        }

        template<typename Func>
        auto then(Func&& func){
            using ReturnType = decltype(func(std::declval<T>()));

            auto new_future = std::async(std::launch::async, [prev_future = _future, func, this] {
                try{
                    T prev_result = prev_future.get();  // This will wait for the previous future
                    return func(prev_result);  // Apply the function to the result
                }catch (std::exception& exception){
                    throw AsyncChainException (exception.what(), _chain_number);
                }

            });
            return ASyncChain<ReturnType>(new_future.share(), _chain_number++);
        }

        T get() const {
            return _future.get();
        }
    };

}
