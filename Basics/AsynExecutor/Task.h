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
        ~Task() override = default;
        explicit Task(TaskFunction&& task): TaskBase(std::move(task)){}

        void execute() override {
            if (_task_function ){
                _task_function();
            }
        }
    };

}
