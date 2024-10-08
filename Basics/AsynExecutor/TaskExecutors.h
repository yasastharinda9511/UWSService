//
// Created by yasas on 8/3/24.
//

#pragma once

#include <thread>
#include <queue>
#include "LockFreeQueue/LockFreeQueue.h"
#include "Task.h"
#include "TimeOutChecker.h"

namespace async {

    class TaskExecutor {

    public:
        explicit TaskExecutor(uint64_t executor_id, TimeoutChecker *timeout_checker) : _running(true),
                                                                                       _async_executor_id(executor_id),
                                                                                       _timeout_checker(timeout_checker) {
            _async_executor = std::thread([this]() {
                while (_running) {
                    if (!_task_queue.is_empty()) {
                        auto *task = _task_queue.dequeue();
                        if (task->get_task_status() == SUSPENDED) {
                            _suspended_task_queue.enqueue(task);
                            continue;
                        }
                        task->execute();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            });
        }

        ~TaskExecutor() {
            _running = false;
            if (_async_executor.joinable()) {
                _async_executor.join();
            }
        }

        void submit(TaskBase *t) {
            _timeout_checker->add_time_out(t->get_task_id(), t);
            _task_queue.enqueue(t);
        }

        uint64_t get_executor() {
            return _async_executor_id;
        }

    private:
        lock_free::LockFreeQueue<TaskBase *> _task_queue;
        lock_free::LockFreeQueue<TaskBase *> _suspended_task_queue;
        std::atomic<bool> _running;
        std::thread _async_executor;
        uint64_t _async_executor_id;
        TimeoutChecker *_timeout_checker;

    };


}