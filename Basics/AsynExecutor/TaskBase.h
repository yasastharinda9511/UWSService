//
// Created by yasas on 9/3/24.
//

#pragma once


#include <cstdint>
#include <unordered_set>
#include <random>

namespace async{

    enum TaskStatus{
        INITIATE,
        RUNNING,
        SUSPENDED,
        EXECUTED
    };

    class TaskBase{
    public:
        TaskBase():_task_id(generate_unique_id()){}
        TaskStatus get_task_status() const {
            return _task_status.load();  // Atomically get the status
        }

        void set_task_status(TaskStatus new_status) {
            _task_status.store(new_status);  // Atomically set the status
        }

    protected:
        uint64_t _task_id;
        static std::unordered_set<uint64_t> used_ids;
        std::atomic<TaskStatus> _task_status = INITIATE;

    private:
        uint64_t generate_unique_id() {
            std::mt19937_64 rng(std::random_device{}());
            std::uniform_int_distribution<uint64_t> dist;
            uint64_t id;
            do {
                id = dist(rng);
            } while (used_ids.find(id) != used_ids.end());

            used_ids.insert(id);
            return id;
        }
    };

}