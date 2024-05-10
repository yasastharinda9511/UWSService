//
// Created by yasas on 4/28/24.
//

#pragma once

#include "BaseController.h"
#include "../Models/Student.h"
#include "../Basics/DataTraveller.h"
#include "../EventBus/EventBus.h"
#include "../Constants/event_bus.h"

namespace Controllers {
    template<bool ssl>
    class StudentController : public BaseController {

    public:
        static void on_add_request(uWS::HttpResponse<ssl> *res, uWS::HttpRequest *req) {
            res->onAborted([](){
                std::cerr << "Request aborted!" << std::endl;
            });

            auto* dt = new Basics::DataTraveller<ssl>(res, Constants::Events::ADD_STUDENT);

            res->onData([dt](std::string_view data, bool last){
                Student s = on_data<Student>(data, last);
                dt->add_data("students", s);
                EventBus::EventBus<Basics::BaseTraveller>::get_event_bus()->publish_async(Constants::EventBusStreams::DB_STREAM, *dt);
            });
        }

        static void get_all_students(uWS::HttpResponse<ssl> *res, uWS::HttpRequest *req){

            res->onAborted([](){
                std::cerr << "Request aborted!" << std::endl;
            });

            auto* dt = new Basics::DataTraveller<ssl>(res, Constants::Events::GET_ALL_STUDENTS);
            EventBus::EventBus<Basics::BaseTraveller>::get_event_bus()->publish_async(Constants::EventBusStreams::DB_STREAM, *dt);

        }
    };

}

