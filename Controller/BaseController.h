//
// Created by yasas on 4/28/24.
//

#pragma once

#include <HttpResponse.h>
#include <nlohmann/json.hpp>

namespace Controllers {

    class BaseController {
    public:
        template<typename T>
        static T* on_data(std::string_view data, bool last) {
            if (last) {
                try {
                    auto json = nlohmann::json::parse(data);
                    T* t  = new T(json);
                    nlohmann::json responseJSON = t->toJSON();
                    return t;

                } catch (const std::exception &e) {
                    return nullptr;
                }
            }
        }
    };
}