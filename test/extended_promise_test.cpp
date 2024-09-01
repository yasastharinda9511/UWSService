//
// Created by yasas on 7/28/24.
//



#include <gtest/gtest.h>
#include "../Basics/AsynExecutor/ExtendedPromise.h"
#include "../Basics/AsynExecutor/AsyncExecutor.h"

int square(int n) {
    return n*n ;
}

void print(int x){
    ASSERT_EQ(x , 10000);
}

TEST(ExtendedPromise , basic) {

    async::AsyncExecutor& executor = async::AsyncExecutor::create();

    auto promise = executor.async_executor_submit(square, 100);

    promise.on_success([](int x){
        print(x);
    })->on_fail([](const std::exception_ptr& ex){
        std::cout << "result is " << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST(ExtendedPromise, compose_test){

    async::AsyncExecutor& executor = async::AsyncExecutor::create();

    async::ExtendedPromise<int> promise  = executor.async_executor_submit(square, 100);

    promise.compose([](int x){
        return x*100;
    })->compose([](int x){
        return x*100;
    })->on_success([](const int & x){
        std::cout << "last value is "<< x << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));
}

