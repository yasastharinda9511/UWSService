//
// Created by yasas on 4/27/24.
//

#pragma once

#include <cstdint>
#include <uWebSockets/App.h>
#include "Constants/HttpMethods.h"

namespace Services{

    class Service {
        using Handler = std::function<void(uWS::HttpResponse<false>* res, uWS::HttpRequest* req)>;
    public:
        void start(uint32_t port);
        Service* add_route(HttpMethods methods, const std::string& path, const Handler& handler );

    private:
        uint32_t _port = 8080;
        uWS::App* _application;
        uWS::Loop *_event_loop;
    };

}

