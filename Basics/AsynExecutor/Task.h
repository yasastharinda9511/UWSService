//
// Created by yasas on 8/2/24.
//

#pragma once

#include <functional>

namespace async{

    class Task{
    public:
        using TaskFunction = std::function<void()>;
        Task() = default;
        explicit Task(TaskFunction&& task): _task_function(std::move(task)){};

        auto execute() {
            if (_task_function){
                _task_function();
            }
        }

    private:
        TaskFunction _task_function;
    };

}
