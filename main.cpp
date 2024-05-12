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
#include "Basics/AsynExecutor/ExtendedPromise.h"
#include "Basics/AsynExecutor/AsyncChain.h"

void print_number(int num) {
    std::cout << "Number: " << num << std::endl;
}

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
        student_service.add_route(HttpMethods::POST, "/addStudent",
                                  Controllers::StudentController<false>::on_add_request)
                ->add_route(HttpMethods::GET, "/getAllStudents",
                            Controllers::StudentController<false>::get_all_students);
        loop = uWS::Loop::get();
        loop->run();
    });

    Basics::Async::ASyncChain<int> chain;  // Start the chain with an initial value of 100

    auto resultChain = chain.start([](int x) { return x + 100; } , 1000)
            .then([](int x){return  x + 100 ;})
            .then([](int x){ return x*x;});
    // Get the final result
    try{
        std::cout << "Result: " << resultChain.get() << std::endl;  // Should output 350.
    }catch (Basics::Async::AsyncChainException& e){
        std::cout << e.what() << std::endl;
    }


    sever_thread.join();

}
