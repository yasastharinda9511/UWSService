//
// Created by yasas on 9/3/24.
//

#pragma once

#include <cstdint>
#include <unordered_set>
#include <random>
#include "TimeUtils.h"

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
        explicit TaskBase(TaskFunction &&task) : _task_function(std::move(task)),
                                                 _created_time(TimeUtils::get_task_created_time()),
                                                 _task_id(calculate_task_id()) {}

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

        virtual void set_time_out(uint64_t time_out) {
            _time_out = time_out;
        }

        [[nodiscard]] bool is_time_out(uint64_t reference_time) const {
            return _created_time + _time_out < reference_time;
        }

        uint64_t get_task_id(){
            return _task_id;
        }

    protected:
        void set_task_status(TaskStatus new_status) {
            std::lock_guard<std::mutex> lock(_task_status_lock);
            _task_status = new_status;  // Atomically set the status
        }

        TaskFunction _task_function{};
    private:
        uint64_t calculate_task_id() {
            return _task_count.fetch_add(1, std::memory_order_relaxed);
        }

        std::mutex _task_status_lock{};
        TaskStatus _task_status = INITIATE;
        uint64_t _created_time;
        uint64_t _time_out = 1000000;
        uint64_t _task_id;
        static std::atomic<uint64_t> _task_count;

    };

    std::atomic<uint64_t> TaskBase::_task_count{0};
}