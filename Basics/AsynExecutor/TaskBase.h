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
        using TaskFunction = std::function<void()>;
        TaskBase(TaskFunction&& task):_task_function(std::move(task)), _task_id(generate_unique_id()){}
        virtual ~TaskBase()= default;

        TaskStatus get_task_status() const {
            return _task_status;  // Atomically get the status
        }

        void set_task_status(TaskStatus new_status) {
            std::lock_guard<std::mutex> lock(_task_status_lock);
            _task_status = new_status;  // Atomically set the status
        }

        uint64_t get_task_id(){
            return _task_id;
        }

        virtual void execute();

    protected:
        uint64_t _task_id;
        static std::unordered_set<uint64_t> used_ids;
        std::mutex _task_status_lock{};
        TaskStatus _task_status = INITIATE;
        TaskFunction _task_function{};

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