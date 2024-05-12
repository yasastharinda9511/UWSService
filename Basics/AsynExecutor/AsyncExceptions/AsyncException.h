//
// Created by yasas on 5/12/24.
//

#pragma once

#include <exception>
#include <string>
#include <cstdint>
#include <utility>

namespace Basics::Async{

    class AsyncChainException: public std::exception{
    public:
        AsyncChainException(const std::string& error_message, uint32_t chain_number):_chain_number(chain_number) , _error_message("Chain " + std::to_string(chain_number) + ": " + error_message){
        }

        [[nodiscard]] const char* what() const noexcept override {
            return _error_message.c_str();
        }

        [[nodiscard]] uint32_t get_error_chain() const noexcept {
            return _chain_number;
        }

    private :
        std::string  _error_message;
        uint32_t _chain_number;
    };
}
