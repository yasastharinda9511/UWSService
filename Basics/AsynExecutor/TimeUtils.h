//
// Created by yasas on 9/15/24.
//
#pragma once

#include <cstdint>
#include <chrono>

namespace async{

    class TimeUtils{

    public:
        static uint64_t get_task_created_time(){
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto milliseconds
                    = std::chrono::duration_cast<std::chrono::milliseconds>(
                            duration)
                            .count();

            return static_cast<uint64_t>(milliseconds);;
        }



    };



}
