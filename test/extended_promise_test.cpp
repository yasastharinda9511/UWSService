//
// Created by yasas on 7/28/24.
//



#include <gtest/gtest.h>
#include "../Basics/AsynExecutor/ExtendedPromise.h"
#include "../Basics/AsynExecutor/AsyncExecutor.h"

int calculate_factorial(int n) {
    if (n < 0) throw std::invalid_argument("Negative number not allowed");
    if (n == 0 || n == 1) return 1;
    return n ;
}

TEST(ExtendedPromise , basic) {

    async::AsyncExecutor asyncExecutor;

    auto promise = asyncExecutor.async_executor_submit(calculate_factorial, 10);

    promise->on_success([](int x){
        std::cout << "result is " << x << std::endl;
    })->on_fail([](const std::exception& ex){
        std::cout << "result is " << ex.what() << std::endl;
    });
    asyncExecutor.async_executor.join();
}

