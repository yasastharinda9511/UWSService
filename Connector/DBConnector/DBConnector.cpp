//
// Created by yasas on 4/29/24.
//

#include <iostream>
#include <utility>
#include "DBConnector.h"
#include "../../EventBus/EventBus.h"

#include "DbScriptHandlers/AddStudentHandler.h"
#include "DbScriptHandlers/GetAllStudentsHandler.h"
#include "../../Constants/event_bus.h"

bool Connectors::DBConnector::connect() {
    try {
        if (_conn->is_open()) {
            return true;
        }else{
            delete _conn;
            _conn = new pqxx::connection(_connection_string);
            if (_conn->is_open()) {
                return true;
            }else {
                return false;
            }
        }
    } catch (const std::exception& e) {
        return false;
    }
}

bool Connectors::DBConnector::disconnect() {
    return false;
}

void Connectors::DBConnector::exec(Basics::BaseTraveller& base_traveller) {

    if (!_conn->is_open()) {
        std::cout << "Connection to database is closed. Cannot execute queries.\n";
    }else{
        try {
            _connector_map.find(base_traveller.get_event())->second->handle(base_traveller, _conn, _sql_store);
            std::cout << "Query executed successfully!\n";
        } catch (const pqxx::sql_error& e) {
            std::cerr << "SQL error: " << e.what() << "\n";
            std::cerr << "Query: " << e.query() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error executing query: " << e.what() << "\n";
        }
    }

    EventBus::EventBus<Basics::BaseTraveller>::get_event_bus()->publish_async(Constants::EventBusStreams::DATA_DISPATCHER_STREAM, base_traveller);
}

Connectors::DBConnector::DBConnector(std::string connection_string, Stores::SQLStore &sql_store) : _connection_string(
        std::move(connection_string)), _sql_store(sql_store) {

    _conn = new pqxx::connection(_connection_string);
    if (_conn->is_open()) {
        std::cout << "success fully connected" << std::endl;
    } else {
        std::cout << "connection fail" << std::endl;
    }
}

void Connectors::DBConnector::init() {

    _connector_map.try_emplace(Constants::Events::ADD_STUDENT, new Connectors::Handlers::AddStudentHandler());
    _connector_map.try_emplace(Constants::Events::GET_ALL_STUDENTS, new Connectors::Handlers::GetAllStudentsHandler());

}
