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
                    if(!_task_queue.is_empty()){
                        auto* task = _task_queue.dequeue();
                        if(task->get_task_status() == SUSPENDED){
                            _suspended_task_queue.enqueue(task);
                            continue;
                        }
                        task->execute();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            });
        }

        ~TaskExecutor(){
            _running = false;
            if(async_executor.joinable()){
                async_executor.join();
            }
        }

        void submit(TaskBase* t){
            _task_queue.enqueue(t);
        }

    private:
        lock_free::LockFreeQueue<TaskBase*> _task_queue;
        lock_free::LockFreeQueue<TaskBase*> _suspended_task_queue;
        std::atomic<bool> _running;
        std::thread async_executor;

    };


}