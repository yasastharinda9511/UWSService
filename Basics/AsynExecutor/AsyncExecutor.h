//
// Created by yasas on 8/1/24.
//

#include <future>
#include <thread>
#include "ExtendedPromise.h"
#include "LockFreeQueue/LockFreeQueue.h"
#include "Task.h"

#pragma once

namespace async{

    class AsyncExecutor{

    public:
        AsyncExecutor() : _running(true){
            async_executor = std::thread([this](){
                while (_running) {
                    Task task;
                    while(this->_task_queue.dequeue(task)){
                        task.execute();
                    }
                }
            });
        }

        ~AsyncExecutor(){
            _running = false;
            if (async_executor.joinable()){
                async_executor.join();
            }
        }

        template<typename Func, typename... Args>
        auto async_executor_submit(Func func, Args&&... args) -> ExtendedPromise<decltype(func(args...))>* {
            using ReturnType = decltype(func(std::forward<Args>(args)...));
            auto* promise = new ExtendedPromise<ReturnType>();

            Task t([promise , func, args...](){
                try{
                    ReturnType value = func(args...);
                    promise->complete(value);
                }catch(...){
                    promise->set_exception(std::current_exception());
                }
            });

            _task_queue.enqueue(t);
            return promise;
        }

    private:
        static AsyncExecutor context;
        lock_free::LockFreeQueue<Task> _task_queue;
        std::atomic<bool> _running;
        std::thread async_executor;

    };

}
