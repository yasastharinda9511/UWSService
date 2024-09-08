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
        auto async_executor_submit(Func func, Args&&... args) -> ExtendedPromise<decltype(func(args...))>* {
            using ReturnType = decltype(func(std::forward<Args>(args)...));

            // Create the promise on the heap and manage it correctly
            auto promise = new ExtendedPromise<ReturnType>();

            // Capture promise and function arguments correctly using move semantics and std::forward
            Task* t = new Task([promise, func = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
                try {
                    // Execute the function with forwarded arguments
                    if constexpr (std::is_void_v<ReturnType>) {
                        func(std::forward<Args>(args)...);  // Call the function
                        promise->complete();               // Complete the promise with no value (void)
                    } else {
                        ReturnType value = func(std::forward<Args>(args)...);  // Call the function and get return value
                        promise->complete(value);                             // Complete the promise with the value
                    }
                } catch (...) {
                    promise->fail(std::current_exception());  // Handle exceptions and fail the promise
                }
            });

            // Submit the task to the executor
            auto task_executor = get_next_executor();
            task_executor->submit(t);

            return promise;
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
                auto* task_executor = new TaskExecutor();
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
