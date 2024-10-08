//
// Created by yasas on 8/1/24.
//

#pragma once


#include <future>
#include <thread>
#include "ExtendedPromise.h"
#include "LockFreeQueue/LockFreeQueue.h"
#include "TaskExecutors.h"
#include "Task.h"
#include "TimeOutChecker.h"

namespace async{

    class AsyncExecutor{

    public:

        static AsyncExecutor& create(int task_executor_count = 1){
            std::call_once(initInstanceFlag, [&]() {
                instance.reset(new AsyncExecutor(task_executor_count));
            });

            return  *instance;
        }

        static AsyncExecutor& get_executor() {
            return *instance;
        }

        template<typename Func, typename... Args>
        auto async_executor_submit(Func func, Args&&... args) -> Task<decltype(func(args...))>* {
            using ReturnType = decltype(func(std::forward<Args>(args)...));
            auto promise = new ExtendedPromise<ReturnType>();

            Task<ReturnType>* t = new Task([promise, func = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
                try {
                    if constexpr (std::is_void_v<ReturnType>) {
                        func(std::forward<Args>(args)...);
                        promise->complete();
                    } else {
                        ReturnType value = func(std::forward<Args>(args)...);
                        promise->complete(value);
                    }
                } catch (...) {
                    promise->fail(std::current_exception());
                }
            } ,promise);
            auto task_executor = get_next_executor();
            task_executor->submit(t);
            return t;
        }

        ~AsyncExecutor(){
            for(auto& [_, executor] : _task_executors){
                delete executor;
            }

        }

    private:

        AsyncExecutor() : _task_executors_count(1) , _submit_index(0){
            initialize_executors(_task_executors_count);
        }

        explicit AsyncExecutor(int task_executor_count) : _task_executors_count(task_executor_count), _submit_index(0) {
            initialize_executors(_task_executors_count);
        }

        void initialize_executors(int count) {
            for (int i = 0; i < count; ++i) {
                auto* timeout_checker = new TimeoutChecker();
                auto* task_executor = new TaskExecutor(i, timeout_checker);
                _task_executors.try_emplace(i, task_executor);
            }
        }

        TaskExecutor* get_next_executor() {
            int index = _submit_index.fetch_add(1, std::memory_order_relaxed) % _task_executors_count;
            return _task_executors[index];
        }

        std::unordered_map <int , TaskExecutor*> _task_executors ;
        int32_t _task_executors_count;
        std::atomic<int32_t> _submit_index;

        static std::unique_ptr<AsyncExecutor> instance;
        static std::once_flag initInstanceFlag;

    };

    std::unique_ptr<AsyncExecutor> AsyncExecutor::instance;
    std::once_flag AsyncExecutor::initInstanceFlag;

}
