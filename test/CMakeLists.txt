cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        /home/yasas/CLionProjects/3rd_party_libs/include
        /home/yasas/CLionProjects/3rd_party_libs/include/uWebSockets
)
link_directories(/home/yasas/CLionProjects/3rd_party_libs/libs
        /home/yasas/CLionProjects/3rd_party_libs/lib64)


add_executable(tests test_main.cpp
        extended_promise_test.cpp
        lock_free_test.cpp)
target_link_libraries(tests gtest)