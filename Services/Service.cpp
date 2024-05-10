//
// Created by yasas on 4/27/24.
//

#include "Service.h"

namespace Services{

    void Service::start(uint32_t port) {

        _application = new uWS::App ();
        _application->listen(port,[&port](auto *token){
            if (token) {
                std::cout << "Server listening on port " <<  port << std::endl;
            } else {
                std::cerr << "Failed to listen on port " << port << std::endl;
            }
        });
    }

    Service *Service::add_route(HttpMethods method, const std::string &path, const Service::Handler& handler) {
        if(method == HttpMethods::GET){
            _application->get(path, handler);
        } else if (method == HttpMethods::POST){
            _application->post(path, handler);
        }else if (method == HttpMethods::PUT){
            _application->put(path, handler);
        }else if (method == HttpMethods::OPTIONS){
            _application->options(path, handler);
        }

        return this;
    }

}
