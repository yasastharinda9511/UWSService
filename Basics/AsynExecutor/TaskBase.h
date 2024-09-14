//
// Created by yasas on 9/3/24.
//

#pragma once

#include <cstdint>
#include <unordered_set>
#include <random>

namespace async {
    using TaskFunction = std::function<void()>;
    enum TaskStatus {
        INITIATE,
        RUNNING,
        RESUME,
        SUSPENDED,
        EXECUTED
    };

    class TaskBase {
    public:
        explicit TaskBase(TaskFunction &&task) : _task_function(std::move(task)) {};

        virtual ~TaskBase() = default;

        [[nodiscard]] TaskStatus get_task_status() const {
            return _task_status;  // Atomically get the status
        }

        void suspend() {
            set_task_status(SUSPENDED);
        }

        void resume() {
            set_task_status(RESUME);
        }

        virtual void execute() = 0;

    protected:
        void set_task_status(TaskStatus new_status) {
            std::lock_guard<std::mutex> lock(_task_status_lock);
            _task_status = new_status;  // Atomically set the status
        }

        std::mutex _task_status_lock{};
        TaskStatus _task_status = INITIATE;
        TaskFunction _task_function{};
    };
}