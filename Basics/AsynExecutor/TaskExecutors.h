//
// Created by yasas on 8/3/24.
//

#pragma once

#include <thread>
#include <queue>
#include "LockFreeQueue/LockFreeQueue.h"
#include "Task.h"

namespace async{

    class TaskExecutor{

    public:
        TaskExecutor():_running(true){
            async_executor = std::thread([this](){
                while (_running) {
                    auto task = this->_task_queue.dequeue();
                    if(task != nullptr){
                        task->execute();
                    }
                }
            });
        }

        ~TaskExecutor(){
            _running = false;
            if(async_executor.joinable()){
                async_executor.join();
            }
        }

        void submit(Task* t){
            _task_queue.enqueue(t);
        }

    private:
        lock_free::LockFreeQueue<Task*> _task_queue;
        std::atomic<bool> _running;
        std::thread async_executor;

    };


}