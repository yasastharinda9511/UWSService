//
// Created by yasas on 4/29/24.
//
#pragma once

#include <string>
#include "../BaseConnector.h"
#include "DbScriptHandlers/BaseHandler.h"
#include "../../Stores/SqlStore.h"
#include <pqxx/pqxx>

namespace Connectors{
    class DBConnector: BaseConnector {

    public:
        DBConnector(std::string  connection_string, Stores::SQLStore& sql_store);
        bool connect() override;
        bool disconnect() override;
        void exec(Basics::BaseTraveller& base_traveller);
        void init() override;
    private:
        pqxx::connection* _conn = nullptr;
        std::string _connection_string;
        std::unordered_map<Constants::Events, Connectors::Handlers::BaseHandler* > _connector_map;
        Stores::SQLStore& _sql_store;
    };
}
