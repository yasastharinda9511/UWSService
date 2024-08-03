//
// Created by yasas on 8/3/24.
//


#include <gtest/gtest.h>
#include "../Basics/AsynExecutor/ExtendedPromise.h"
#include "../Basics/AsynExecutor/LockFreeQueue/LockFreeQueue.h"


TEST(LockFreeQueue , basic) {

    async::lock_free::LockFreeQueue<int> queue;

    std::thread t_1([&queue](){
        for(int i = 0 ; i < 10000 ; i ++){
            queue.enqueue(i);
        }
    });

    std::thread t_2([&queue](){
        for(int i = 10000 ; i < 20000 ; i ++){
            queue.enqueue(i);
        }
    });

    std::thread t_3([&queue](){
        for(int i = 20000 ; i < 30000 ; i ++){
            queue.enqueue(i);
        }
    });

    t_1.join();
    t_2.join();
    t_3.join();

    std::promise<int> t4_promise ;
    std::future<int> t4_future = t4_promise.get_future();

    std::thread t_4([&queue, &t4_promise](){
        int value;
        int count =0 ;
        while (queue.dequeue(value)) {
            count++;
        }
        t4_promise.set_value(count);
    });

    std::promise<int> t5_promise ;
    std::future<int> t5_future = t5_promise.get_future();

    std::thread t_5([&queue, &t5_promise](){
        int value;
        int count =0 ;
        while (queue.dequeue(value)) {
            count++;
        }
        t5_promise.set_value(count);
    });

    t_4.join();
    t_5.join();

    int total = t4_future.get() + t5_future.get() ;
//
    ASSERT_EQ(total , 30000);

}