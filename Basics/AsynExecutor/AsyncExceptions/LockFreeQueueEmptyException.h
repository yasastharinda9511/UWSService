//
// Created by yasas on 9/8/24.
//

#pragma once

#include <exception>
#include <string>
#include <cstdint>
#include <utility>

namespace Basics::Async{

    class LockFreeQueueEmptyException: public std::exception{
    public:
        LockFreeQueueEmptyException() = default;

        [[nodiscard]] const char* what() const noexcept override {
            return _error_message.c_str();
        }

    private :
        std::string  _error_message = "Queue is Empty";
    };
}