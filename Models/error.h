//
// Created by yasas on 5/1/24.
//

#pragma once

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

namespace Models{

    class Error{

    private:
        int32_t _error_code;
        std::string _error_message ;

    public:
        Error(int32_t error_code , std::string error_message):_error_code(error_code), _error_message(error_message){}

        [[nodiscard]] std::string to_JSON() const {
            return to_string(nlohmann::json{{"error_code", _error_code},{"error_message", _error_message}});
        }


    };


}