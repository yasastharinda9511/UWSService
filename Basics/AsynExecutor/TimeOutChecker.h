//
// Created by yasas on 9/14/24.
//
#pragma once


#include <unordered_map>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <iostream>
#include "TaskBase.h"
#include "TimeUtils.h"
#include "TaskExecutors.h"

namespace async {

    class TaskExecutor;

    class TimeoutChecker {

    public:
        TimeoutChecker() {
            _checker_thread = std::thread([this]() { remove_timeout_tasks(); });
        }

        void add_time_out(const uint64_t &task_id, TaskBase *task) {
            _timout_queue_access.lock();
            _timeout_queue.try_emplace(task_id, task);
            _timout_queue_access.unlock();
        }

        void erase_task_id(const uint64_t &task_id) {
            _timout_queue_access.lock();
            auto ite = _timeout_queue.find(task_id);
            if(ite != _timeout_queue.end()){
                _timeout_queue.erase(ite);
            }
            _timout_queue_access.unlock();
        }

    private:
        void remove_timeout_tasks() {
            while (_running) {
                auto current_time = TimeUtils::get_task_created_time();
                std::lock_guard<std::mutex> lock(_timout_queue_access);
                for (auto it = _timeout_queue.begin(); it != _timeout_queue.end();) {
                    if (it->second->is_time_out(current_time)) {
                        std::cout << "time out occurred" << std::endl;
                        it = _timeout_queue.erase(it); // Remove the timed-out task
                    } else {
                        ++it;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

    private:
        std::unordered_map<uint64_t, TaskBase *> _timeout_queue;
        std::mutex _timout_queue_access;
        std::atomic<bool> _running = true;
        std::thread _checker_thread;
    };
}
