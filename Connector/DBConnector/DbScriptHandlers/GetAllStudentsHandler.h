//
// Created by yasas on 5/1/24.
//

#pragma once


#include "BaseHandler.h"
#include "../../../Models/Student.h"
#include "../../../Constants/errorCodes.h"

namespace Connectors::Handlers {
    class GetAllStudentsHandler : public BaseHandler {

    public:
        void handle(Basics::BaseTraveller& dataTraveller ,  pqxx::connection* conn, const Stores:: SQLStore& sql_store) override {

            std::string sql = sql_store.get_query(Constants::SQLQueryTypes::GET_ALL_STUDENTS);

            if (!conn->is_open()) {
                std::cout << "Connection to database is closed. Cannot execute queries.\n";
                return;
            }
            try {
                pqxx::work txn(*conn);
                pqxx::result result = txn.exec(sql);

                for(const auto& row : result ){
                    int id = row["id"].as<int>(); // Accessing by column name
                    auto student_name = row["student_name"].as<std::string>();
                    int age = row["age"].as<int>();
                    auto email = row["email"].as<std::string>();
                    auto stream = row["stream"].as<std::string>();

                    auto* response = new Student(id, student_name, email,age, stream);

                    dataTraveller.add_data("response_data", response);

                }
                txn.commit();

            } catch (const pqxx::sql_error& e) {
                dataTraveller.set_error_code(static_cast<int32_t>(Constants::ErrorCodes::SQL_ERROR_CODE));
                dataTraveller.set_error_msg(e.what());
            } catch (const std::exception& e) {
                dataTraveller.set_error_code(static_cast<int32_t>(Constants::ErrorCodes::DATA_TRAVELLER_DATE_RETRIEVING_ERROR_CODE));
                dataTraveller.set_error_msg(e.what());
            }

        }



    };

}