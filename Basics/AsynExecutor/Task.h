//
// Created by yasas on 8/2/24.
//

#pragma once

#include <functional>
#include <random>
#include <unordered_set>
#include "TaskBase.h"

namespace async{

    template<typename T>
    class Task:public TaskBase{
    public:
        ~Task() override{ delete _task_promise; }
        explicit Task(TaskFunction&& task, ExtendedPromise<T> *task_promise): TaskBase(std::move(task)), _task_promise(task_promise){}

        void execute() override {
            if (this->_task_function){
                this->set_task_status(RUNNING);
                this->_task_function();
                this->set_task_status(EXECUTED);
            }
        }

        ExtendedPromise<T>& get_promise(){
            return *_task_promise;
        }

        virtual Task<T>* on_success(std::function<void(const T &)> on_success) {
            _task_promise->on_success(on_success);
            return this;
        }


        virtual Task<T>* on_fail(std::function<void(const std::exception_ptr)> callback){
            _task_promise->on_fail(callback);
            return this;
        }


    private:
        ExtendedPromise<T> *_task_promise{};
    };

}
