//
// Created by yasas on 4/28/24.
//
#pragma once

#include <unordered_map>
#include <string>
#include <HttpResponse.h>
#include <future>
#include "BaseTraveller.h"


namespace Basics{
    template<bool ssl>
    class DataTraveller : public BaseTraveller{

    public:
        DataTraveller( uWS::HttpResponse<ssl> *response, Constants::Events event):BaseTraveller(event),_response(response){}
        uWS::HttpResponse<ssl> * get_response(){ return _response ;}

    private:
        uWS::HttpResponse<ssl> *_response;
    };
}


