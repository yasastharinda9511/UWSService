//
// Created by yasas on 5/1/24.
//

#pragma once


#include "BaseResponseMapperHandler.h"
#include "../../Models/error.h"

namespace Finalizer:: ResponseMapHandlers{

    class ErrorHandler: public BaseResponseMapHandler {
    public:
        std::string handel_response(Basics::BaseTraveller &data_traveller) override {

            int32_t error_code = data_traveller.get_error_code();
            std::string error_message = data_traveller.get_error_message();

            Models::Error error(error_code, error_message);

            std::string json_response = error.to_JSON();
            return json_response;
        }

    };

}