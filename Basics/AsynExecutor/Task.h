//
// Created by yasas on 8/2/24.
//

#pragma once

#include <functional>
#include <random>
#include <unordered_set>
#include "TaskBase.h"

namespace async{

    class Task:public TaskBase{
    public:
        using TaskFunction = std::function<void()>;
        Task() = default;
        explicit Task(TaskFunction&& task):TaskBase(), _task_function(std::move(task)){
        };

        auto execute() {
            if (_task_function && get_task_status() != SUSPENDED){
                set_task_status(RUNNING);
                _task_function();
                set_task_status(EXECUTED);
            }
        }

    private:
        TaskFunction _task_function;
    };

}
