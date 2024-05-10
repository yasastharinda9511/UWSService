#include <iostream>
#include "uWebSockets/App.h"
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>

#include "Services/Service.h"
#include "Models/Student.h"
#include "Controller/StudentController.h"
#include "EventBus/EventBus.h"
#include "Basics/DataTraveller.h"
#include "Connector/DBConnector/DBConnector.h"
#include "DataFinalizer/DataFinalizer.h"
#include "Constants/event_bus.h"


int main() {

    Services::Service student_service;

    std::string connection_string = "postgresql://my_user:my_password@localhost:5432/my_database";
    Stores::SQLStore sql_store;
    Connectors::DBConnector dbConnector(connection_string, sql_store);
    dbConnector.connect();
    dbConnector.init();
    Finalizer::DataFinalizer data_dispatcher;

    EventBus::EventBus<Basics::BaseTraveller>::get_event_bus()->subscribe(Constants::EventBusStreams::DB_STREAM, std::bind(
            &Connectors::DBConnector::exec, &dbConnector, std::placeholders::_1));

    EventBus::EventBus<Basics::BaseTraveller>::get_event_bus()->subscribe(Constants::EventBusStreams::DATA_DISPATCHER_STREAM, std::bind(
            & Finalizer::DataFinalizer::listen, &data_dispatcher, std::placeholders::_1));

    uWS::Loop *loop = nullptr;

    std::thread sever_thread([&]() {
        student_service.start(3000);
        student_service.add_route(HttpMethods::GET, "/getStudent",
                                  Controllers::StudentController<false>::on_add_request)
                ->add_route(HttpMethods::GET, "/getAllStudents",
                            Controllers::StudentController<false>::get_all_students);
        loop = uWS::Loop::get();
        loop->run();
    });

    sever_thread.join();

}
