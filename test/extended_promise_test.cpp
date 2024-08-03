//
// Created by yasas on 7/28/24.
//



#include <gtest/gtest.h>
#include "../Basics/AsynExecutor/ExtendedPromise.h"
#include "../Basics/AsynExecutor/AsyncExecutor.h"

int calculate_factorial(int n) {
    if (n < 0) throw std::invalid_argument("Negative number not allowed");
    if (n == 0 || n == 1) return 1;
    return n*n ;
}

void print(int x){
    std::cout << "result is " << x << std::endl;
}

TEST(ExtendedPromise , basic) {

    async::AsyncExecutor& executor = async::AsyncExecutor::create();

    auto promise = executor.async_executor_submit(calculate_factorial, 100);

    promise.on_success([](int x){
        print(x);
    })->on_fail([](const std::exception_ptr ex){
        std::cout << "result is " << std::endl;
    });

    std::cout << promise._future.get() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

