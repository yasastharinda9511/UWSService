//
// Created by yasas on 4/30/24.
//

#pragma once

#include <map>
#include <string>
#include "../Constants/sql_query_types.h"

namespace Stores{

    class SQLStore{
        using QueryTypes = Constants::SQLQueryTypes;
    public :
        SQLStore(){
            _queries[QueryTypes::INSERT_STUDENT_QUERY] = "INSERT INTO students (student_name, age, email, stream) VALUES ('{0}', {1}, '{2}', '{3}') RETURNING id, student_name, age, email, stream;";
            _queries[QueryTypes::GET_ALL_STUDENTS] = "SELECT  * FROM students";
        }

        [[nodiscard]] const std::string& get_query(const Constants::SQLQueryTypes& query_type) const {
            auto ite = _queries.find(query_type);

            if(ite != _queries.end()){
                return ite->second;
            }

            return "";
        }

    private:
        std::map<Constants::SQLQueryTypes, std::string> _queries;

    };



}
