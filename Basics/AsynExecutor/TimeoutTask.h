//
// Created by yasas on 9/8/24.
//

#pragma once


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